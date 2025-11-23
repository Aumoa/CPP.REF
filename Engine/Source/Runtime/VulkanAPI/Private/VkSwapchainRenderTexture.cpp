// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkSwapchainRenderTexture.h"
#include "VkSwapchainExt.h"
#include "VkGraphics.h"
#include "VkCommandBuffer.h"

namespace Ayla
{
	VkSwapchainRenderTexture::VkSwapchainRenderTexture(VkSwapchainExt* swapchain)
		: m_Swapchain(swapchain)
	{
		VkSemaphoreCreateInfo semaphoreCreateInfo{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
		VKR(vkCreateSemaphore(swapchain->GetOwner()->GetDevice(), &semaphoreCreateInfo, nullptr, &m_ImageReadySemaphore));
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
		VKR(vkAcquireNextImageKHR(graphics->GetDevice(), m_Swapchain->GetSwapchain(), UINT64_MAX, m_ImageReadySemaphore, VK_NULL_HANDLE, &m_CurrentImageIndex));

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
		vkCmd->AddImageReadySemaphore(m_ImageReadySemaphore);
	}

	void VkSwapchainRenderTexture::Invalidate()
	{
		m_SwapchainImages.clear();
	}

	void VkSwapchainRenderTexture::Present(VkQueue queue, VkCommandBuffer* vkCmd)
	{
		check(m_CurrentImageIndex != 0xFFFFFFFF);
		auto semaphore = vkCmd->GetRenderCompletedSemaphore();
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