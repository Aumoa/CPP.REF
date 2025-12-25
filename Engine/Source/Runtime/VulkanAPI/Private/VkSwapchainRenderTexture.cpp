// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkSwapchainRenderTexture.h"
#include "VkSwapchainExt.h"
#include "VkGraphics.h"
#include "VkCommandBuffer.h"
#include "VkRaytracingSceneRenderer.h"
#include "Rendering/SceneView.h"

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

		// Store the command buffer for later use in RenderRaytracing
		m_CurrentCommandBuffer = cmd;

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
		auto device = m_Graphics->GetDevice();

		// Clean up raytracing renderer
		m_RaytracingRenderer.reset();

		for (auto& imageView : m_SwapchainImageViews)
		{
			vkDestroyImageView(device, imageView, nullptr);
		}
		m_SwapchainImageViews.clear();

		for (auto& semaphore : m_PresentCompletedSemaphores)
		{
			vkDestroySemaphore(device, semaphore, nullptr);
		}
		m_PresentCompletedSemaphores.clear();

		for (auto& semaphore : m_RenderCompletedSemaphores)
		{
			vkDestroySemaphore(device, semaphore, nullptr);
		}
		m_RenderCompletedSemaphores.clear();
	}

	void VkSwapchainRenderTexture::RenderRaytracing(CommandBuffer* cmd, const SceneView& view)
	{
		// Initialize raytracing renderer on first use
		if (!m_RaytracingRenderer)
		{
			m_RaytracingRenderer = std::make_unique<VkRaytracingSceneRenderer>(m_Graphics);
		}

		// Use the command buffer from Acquire if cmd is null, otherwise use the provided one
		// This relies on Acquire() being called first to set m_CurrentCommandBuffer
		CommandBuffer* activeCmd = cmd ? cmd : m_CurrentCommandBuffer;
		check(activeCmd != nullptr);

		// Render using Vulkan raytracing
		// TODO: Use SceneView parameters (camera position, rotation, FOV) in raytracing shaders
		m_RaytracingRenderer->Render(
			activeCmd,
			GetCurrentImage(),
			GetCurrentImageView(),
			GetSize()
		);
	}

	void VkSwapchainRenderTexture::Invalidate()
	{
		auto device = m_Graphics->GetDevice();
		
		for (auto& imageView : m_SwapchainImageViews)
		{
			vkDestroyImageView(device, imageView, nullptr);
		}
		m_SwapchainImageViews.clear();
		
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

		// Create image views
		m_SwapchainImageViews.resize(imageCount);
		for (uint32_t i = 0; i < imageCount; ++i)
		{
			VkImageViewCreateInfo createInfo
			{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.image = m_SwapchainImages[i],
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = m_Swapchain->GetFormat(),
				.components = {
					.r = VK_COMPONENT_SWIZZLE_IDENTITY,
					.g = VK_COMPONENT_SWIZZLE_IDENTITY,
					.b = VK_COMPONENT_SWIZZLE_IDENTITY,
					.a = VK_COMPONENT_SWIZZLE_IDENTITY
				},
				.subresourceRange = {
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1
				}
			};

			VKR(vkCreateImageView(device, &createInfo, nullptr, &m_SwapchainImageViews[i]));
		}
	}
}