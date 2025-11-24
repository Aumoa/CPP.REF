// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkSwapchainRenderTexture.h"
#include "VkSwapchainExt.h"
#include "VkGraphics.h"
#include "VkCommandBuffer.h"

namespace Ayla
{
	VkSwapchainRenderTexture::VkSwapchainRenderTexture(VkSwapchainExt* swapchain, VkGraphics* graphics)
		: m_Swapchain(swapchain)
		, m_Graphics(graphics)
	{
		VkSemaphoreCreateInfo semaphoreCreateInfo{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

		m_PresentCompletedSemaphores.resize(VkGraphics::kMaxFramesInFlight);
		for (size_t i = 0; i < VkGraphics::kMaxFramesInFlight; ++i)
		{
			VKR(vkCreateSemaphore(swapchain->GetOwner()->GetDevice(), &semaphoreCreateInfo, nullptr, &m_PresentCompletedSemaphores[i]));
			std::string debugName = String::Format(TEXT("m_PresentCompletedSemaphores[{}]"), i).AsCodepage();
			VkDebugUtilsObjectNameInfoEXT info =
			{
				.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
				.objectType = VK_OBJECT_TYPE_SEMAPHORE,
				.objectHandle = reinterpret_cast<uint64_t>(m_PresentCompletedSemaphores[i]),
				.pObjectName = debugName.c_str(),
			};
			VKR(graphics->GetSetDebugUtilsObjectNameEXTFunction()(graphics->GetDevice(), &info));
		}

		m_RenderCompletedSemaphores.resize(VkGraphics::kMaxSwapchainImages);
		for (size_t i = 0; i < VkGraphics::kMaxSwapchainImages; ++i)
		{
			VKR(vkCreateSemaphore(swapchain->GetOwner()->GetDevice(), &semaphoreCreateInfo, nullptr, &m_RenderCompletedSemaphores[i]));
			std::string debugName = String::Format(TEXT("m_RenderCompletedSemaphores[{}]"), i).AsCodepage();
			VkDebugUtilsObjectNameInfoEXT info =
			{
				.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
				.objectType = VK_OBJECT_TYPE_SEMAPHORE,
				.objectHandle = reinterpret_cast<uint64_t>(m_RenderCompletedSemaphores[i]),
				.pObjectName = debugName.c_str(),
			};
			VKR(graphics->GetSetDebugUtilsObjectNameEXTFunction()(graphics->GetDevice(), &info));
		}
	}

	Vector2N VkSwapchainRenderTexture::GetSize() const
	{
		return m_Swapchain->GetSize();
	}

	void VkSwapchainRenderTexture::Acquire(CommandBuffer* cmd)
	{
		if (m_SwapchainImages.size() == 0)
		{
			ReallocateSwapchainImages();
		}

		check(m_CurrentImageIndex == 0xFFFFFFFF);

		auto graphics = m_Swapchain->GetOwner();
		auto imageReadySemaphore = m_PresentCompletedSemaphores[m_Graphics->GetFrameIndex()];
		VKR(vkAcquireNextImageKHR(graphics->GetDevice(), m_Swapchain->GetSwapchain(), UINT64_MAX, imageReadySemaphore, VK_NULL_HANDLE, &m_CurrentImageIndex));

		auto* vkCmd = (VkCommandBuffer*)cmd;

		bool isFirstRender = (m_SwapchainImageFirstRender & (1 << m_CurrentImageIndex)) == 0;
		if (isFirstRender)
		{
			VkImageMemoryBarrier barrier
			{
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
				.dstAccessMask = 0,
				.oldLayout = isFirstRender ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image = m_SwapchainImages[m_CurrentImageIndex],
				.subresourceRange =
				{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			};

			vkCmdPipelineBarrier(
				vkCmd->GetVkCommandBuffer(),
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				0,
				0, nullptr,
				0, nullptr,
				1, &barrier
			);
		}

		m_SwapchainImageFirstRender |= (1 << m_CurrentImageIndex);
		vkCmd->AddSignalSemaphore(m_RenderCompletedSemaphores[m_CurrentImageIndex]);
		vkCmd->AddWaitSemaphore(m_PresentCompletedSemaphores[m_Graphics->GetFrameIndex()]);
	}

	void VkSwapchainRenderTexture::Dispose()
	{
		for (auto& semaphore : m_PresentCompletedSemaphores)
		{
			vkDestroySemaphore(m_Graphics->GetDevice(), semaphore, nullptr);
		}
		m_PresentCompletedSemaphores.clear();

		for (auto& semaphore : m_RenderCompletedSemaphores)
		{
			vkDestroySemaphore(m_Graphics->GetDevice(), semaphore, nullptr);
		}
		m_RenderCompletedSemaphores.clear();
	}

	void VkSwapchainRenderTexture::Invalidate()
	{
		m_SwapchainImages.clear();
	}

	void VkSwapchainRenderTexture::Present(VkQueue queue, VkCommandBuffer* vkCmd)
	{
		check(m_CurrentImageIndex != 0xFFFFFFFF);
		auto semaphore = m_RenderCompletedSemaphores[m_CurrentImageIndex];
		auto swapchain = m_Swapchain->GetSwapchain();
		VkPresentInfoKHR presentInfo
		{
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &semaphore,
			.swapchainCount = 1,
			.pSwapchains = &swapchain,
			.pImageIndices = &m_CurrentImageIndex
		};

		VKR(vkQueuePresentKHR(queue, &presentInfo), VK_ERROR_SURFACE_LOST_KHR, VK_ERROR_OUT_OF_DATE_KHR);
		m_CurrentImageIndex = 0xFFFFFFFF;
	}

	void VkSwapchainRenderTexture::ReallocateSwapchainImages()
	{
		auto owner = m_Swapchain->GetOwner();
		auto device = owner->GetDevice();
		uint32_t imageCount = 0;
		auto swapchain = m_Swapchain->GetSwapchain();
		VKR(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr));
		m_SwapchainImages.resize(imageCount);
		VKR(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, m_SwapchainImages.data()));
	}
}