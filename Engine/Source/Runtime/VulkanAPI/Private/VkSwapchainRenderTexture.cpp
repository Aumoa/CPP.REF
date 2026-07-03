// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkSwapchainRenderTexture.h"
#include "VkSwapchainExt.h"
#include "VkGraphics.h"
#include "VkCommandBuffer.h"

namespace Ayla
{
	namespace
	{
		constexpr VkPipelineStageFlags kAcquireWaitStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
	}

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

		CreateRenderPass();
	}

	Vector2N VkSwapchainRenderTexture::GetSize() const
	{
		return m_Swapchain->GetSize();
	}

	PresentableFrame VkSwapchainRenderTexture::AcquireFrame(CommandBuffer* commandBuffer)
	{
		if (m_Swapchain->IsPresentable() == false)
		{
			return {};
		}

		if (m_SwapchainImages.size() == 0)
		{
			ReallocateSwapchainImages();
		}

		check(m_CurrentImageIndex == 0xFFFFFFFF);

		auto graphics = m_Swapchain->GetOwner();
		auto imageReadySemaphore = m_PresentCompletedSemaphores[m_Graphics->GetFrameIndex()];
		VkResult acquireResult = vkAcquireNextImageKHR(graphics->GetDevice(), m_Swapchain->GetSwapchain(), UINT64_MAX, imageReadySemaphore, VK_NULL_HANDLE, &m_CurrentImageIndex);
		if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
		{
			m_CurrentImageIndex = 0xFFFFFFFF;
			m_Swapchain->RequestRecreate();
			return {};
		}
		VKR(acquireResult, VK_SUBOPTIMAL_KHR);
		if (acquireResult == VK_SUBOPTIMAL_KHR)
		{
			m_Swapchain->RequestRecreate();
		}

		auto* vkCmd = (VkCommandBuffer*)commandBuffer;

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
		vkCmd->AddWaitSemaphore(m_PresentCompletedSemaphores[m_Graphics->GetFrameIndex()], kAcquireWaitStage);
		return PresentableFrame(this, this);
	}

	void VkSwapchainRenderTexture::Dispose()
	{
		DestroyFramebufferResources();

		if (m_RenderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(m_Graphics->GetDevice(), m_RenderPass, nullptr);
			m_RenderPass = VK_NULL_HANDLE;
		}

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
		DestroyFramebufferResources();
		m_SwapchainImages.clear();
		m_CurrentImageIndex = 0xFFFFFFFF;
		m_SwapchainImageFirstRender = 0;
	}

	void VkSwapchainRenderTexture::Present(CommandBuffer* commandBuffer)
	{
		PLATFORM_UNREFERENCED_PARAMETER(commandBuffer);
		check(m_CurrentImageIndex != 0xFFFFFFFF);
		auto semaphore = m_RenderCompletedSemaphores[m_CurrentImageIndex];
		auto swapchain = m_Swapchain->GetSwapchain();
		uint32 imageIndex = m_CurrentImageIndex;
		VkPresentInfoKHR presentInfo
		{
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &semaphore,
			.swapchainCount = 1,
			.pSwapchains = &swapchain,
			.pImageIndices = &imageIndex
		};

		VkResult presentResult = vkQueuePresentKHR(m_Swapchain->GetPresentQueue(), &presentInfo);
		m_CurrentImageIndex = 0xFFFFFFFF;
		if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR)
		{
			m_Swapchain->RequestRecreate();
			return;
		}
		VKR(presentResult, VK_ERROR_SURFACE_LOST_KHR);
	}

	VkFramebuffer VkSwapchainRenderTexture::GetCurrentFramebuffer() const noexcept
	{
		if (m_CurrentImageIndex < m_Framebuffers.size())
		{
			return m_Framebuffers[m_CurrentImageIndex];
		}
		return VK_NULL_HANDLE;
	}

	VkImage VkSwapchainRenderTexture::GetCurrentImage() const noexcept
	{
		if (m_CurrentImageIndex < m_SwapchainImages.size())
		{
			return m_SwapchainImages[m_CurrentImageIndex];
		}
		return VK_NULL_HANDLE;
	}

	VkImageView VkSwapchainRenderTexture::GetCurrentImageView() const noexcept
	{
		if (m_CurrentImageIndex < m_SwapchainImageViews.size())
		{
			return m_SwapchainImageViews[m_CurrentImageIndex];
		}
		return VK_NULL_HANDLE;
	}

	void VkSwapchainRenderTexture::ReallocateSwapchainImages()
	{
		DestroyFramebufferResources();

		auto owner = m_Swapchain->GetOwner();
		auto device = owner->GetDevice();
		uint32_t imageCount = 0;
		auto swapchain = m_Swapchain->GetSwapchain();
		VKR(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr));
		m_SwapchainImages.resize(imageCount);
		VKR(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, m_SwapchainImages.data()));

		m_SwapchainImageViews.resize(imageCount);
		for (uint32_t i = 0; i < imageCount; ++i)
		{
			VkImageViewCreateInfo viewInfo
			{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.image = m_SwapchainImages[i],
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = kColorFormat,
				.components =
				{
					.r = VK_COMPONENT_SWIZZLE_IDENTITY,
					.g = VK_COMPONENT_SWIZZLE_IDENTITY,
					.b = VK_COMPONENT_SWIZZLE_IDENTITY,
					.a = VK_COMPONENT_SWIZZLE_IDENTITY,
				},
				.subresourceRange =
				{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				}
			};
			VKR(vkCreateImageView(device, &viewInfo, nullptr, &m_SwapchainImageViews[i]));
		}

		CreateDepthResources();
		CreateRaytracingOutputResources();
		CreateFramebuffers();
	}

	void VkSwapchainRenderTexture::CreateRenderPass()
	{
		VkAttachmentDescription attachments[2] =
		{
			// Color attachment
			{
				.format = kColorFormat,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			},
			// Depth attachment
			{
				.format = kDepthFormat,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
			}
		};

		VkAttachmentReference colorRef
		{
			.attachment = 0,
			.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		};

		VkAttachmentReference depthRef
		{
			.attachment = 1,
			.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		};

		VkSubpassDescription subpass
		{
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.colorAttachmentCount = 1,
			.pColorAttachments = &colorRef,
			.pDepthStencilAttachment = &depthRef,
		};

		VkSubpassDependency dependency
		{
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = 0,
			.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
			.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
		};

		VkRenderPassCreateInfo renderPassInfo
		{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.attachmentCount = 2,
			.pAttachments = attachments,
			.subpassCount = 1,
			.pSubpasses = &subpass,
			.dependencyCount = 1,
			.pDependencies = &dependency,
		};

		VKR(vkCreateRenderPass(m_Graphics->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass));
	}

	void VkSwapchainRenderTexture::CreateDepthResources()
	{
		auto device = m_Graphics->GetDevice();
		auto size = GetSize();

		VkImageCreateInfo imageInfo
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = kDepthFormat,
			.extent = { (uint32_t)size.X, (uint32_t)size.Y, 1 },
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		};

		VKR(vkCreateImage(device, &imageInfo, nullptr, &m_DepthImage));

		VkMemoryRequirements memReq;
		vkGetImageMemoryRequirements(device, m_DepthImage, &memReq);

		VkMemoryAllocateInfo allocInfo
		{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize = memReq.size,
			.memoryTypeIndex = m_Graphics->FindMemoryType(memReq.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		};

		VKR(vkAllocateMemory(device, &allocInfo, nullptr, &m_DepthMemory));
		VKR(vkBindImageMemory(device, m_DepthImage, m_DepthMemory, 0));

		VkImageViewCreateInfo viewInfo
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = m_DepthImage,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = kDepthFormat,
			.subresourceRange =
			{
				.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			}
		};

		VKR(vkCreateImageView(device, &viewInfo, nullptr, &m_DepthImageView));
	}

	void VkSwapchainRenderTexture::CreateRaytracingOutputResources()
	{
		auto device = m_Graphics->GetDevice();
		auto size = GetSize();

		VkImageCreateInfo imageInfo
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = kRaytracingOutputFormat,
			.extent = { (uint32_t)size.X, (uint32_t)size.Y, 1 },
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		};

		VKR(vkCreateImage(device, &imageInfo, nullptr, &m_RaytracingOutputImage));

		VkMemoryRequirements memReq;
		vkGetImageMemoryRequirements(device, m_RaytracingOutputImage, &memReq);

		VkMemoryAllocateInfo allocInfo
		{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize = memReq.size,
			.memoryTypeIndex = m_Graphics->FindMemoryType(memReq.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		};

		VKR(vkAllocateMemory(device, &allocInfo, nullptr, &m_RaytracingOutputMemory));
		VKR(vkBindImageMemory(device, m_RaytracingOutputImage, m_RaytracingOutputMemory, 0));

		VkImageViewCreateInfo viewInfo
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = m_RaytracingOutputImage,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = kRaytracingOutputFormat,
			.subresourceRange =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			}
		};

		VKR(vkCreateImageView(device, &viewInfo, nullptr, &m_RaytracingOutputImageView));
		m_RaytracingOutputImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	}

	void VkSwapchainRenderTexture::CreateFramebuffers()
	{
		auto device = m_Graphics->GetDevice();
		auto size = GetSize();

		m_Framebuffers.resize(m_SwapchainImages.size());
		for (size_t i = 0; i < m_SwapchainImages.size(); ++i)
		{
			VkImageView attachments[2] = { m_SwapchainImageViews[i], m_DepthImageView };

			VkFramebufferCreateInfo framebufferInfo
			{
				.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				.renderPass = m_RenderPass,
				.attachmentCount = 2,
				.pAttachments = attachments,
				.width = (uint32_t)size.X,
				.height = (uint32_t)size.Y,
				.layers = 1,
			};

			VKR(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_Framebuffers[i]));
		}
	}

	void VkSwapchainRenderTexture::DestroyFramebufferResources()
	{
		auto device = m_Graphics->GetDevice();

		for (auto& fb : m_Framebuffers)
		{
			vkDestroyFramebuffer(device, fb, nullptr);
		}
		m_Framebuffers.clear();

		if (m_DepthImageView != VK_NULL_HANDLE)
		{
			vkDestroyImageView(device, m_DepthImageView, nullptr);
			m_DepthImageView = VK_NULL_HANDLE;
		}

		if (m_DepthImage != VK_NULL_HANDLE)
		{
			vkDestroyImage(device, m_DepthImage, nullptr);
			m_DepthImage = VK_NULL_HANDLE;
		}

		if (m_DepthMemory != VK_NULL_HANDLE)
		{
			vkFreeMemory(device, m_DepthMemory, nullptr);
			m_DepthMemory = VK_NULL_HANDLE;
		}

		if (m_RaytracingOutputImageView != VK_NULL_HANDLE)
		{
			vkDestroyImageView(device, m_RaytracingOutputImageView, nullptr);
			m_RaytracingOutputImageView = VK_NULL_HANDLE;
		}

		if (m_RaytracingOutputImage != VK_NULL_HANDLE)
		{
			vkDestroyImage(device, m_RaytracingOutputImage, nullptr);
			m_RaytracingOutputImage = VK_NULL_HANDLE;
		}

		if (m_RaytracingOutputMemory != VK_NULL_HANDLE)
		{
			vkFreeMemory(device, m_RaytracingOutputMemory, nullptr);
			m_RaytracingOutputMemory = VK_NULL_HANDLE;
		}
		m_RaytracingOutputImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		for (auto& view : m_SwapchainImageViews)
		{
			vkDestroyImageView(device, view, nullptr);
		}
		m_SwapchainImageViews.clear();
	}
}
