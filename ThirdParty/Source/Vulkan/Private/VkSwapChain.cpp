// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "VkSwapChain.h"
#include "VkDevice.h"
#include "VkFactory.h"
#include "VkImage.h"
#include "VkCommandQueue.h"

GENERATE_BODY(SVkSwapChain);

SVkSwapChain::SVkSwapChain(SVkDevice* Owner, VkSurfaceKHR vkSurface, size_t NumBuffers, VkQueue vkQueue)
	: Super(Owner)
	, vkSurface(std::move(vkSurface))
	, vkQueue(std::move(vkQueue))
{
	vkInstance = Cast<SVkFactory>(Owner->GetFactory())->GetVkInstance();
	vkDevice = Owner->vkDevice;
	SwapChainImages.resize(NumBuffers);

	VkCommandPoolCreateInfo vkCommandPoolCreateInfo =
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.queueFamilyIndex = 0
	};

	VR(vkCreateCommandPool(vkDevice, &vkCommandPoolCreateInfo, nullptr, &vkCommandPool));
}

bool SVkSwapChain::IsReady()
{
	if (vkFence == VK_NULL_HANDLE)
	{
		return true;
	}

	return vkGetFenceStatus(vkDevice, vkFence) != VK_NOT_READY;
}

void SVkSwapChain::Wait()
{
	if (vkFence == VK_NULL_HANDLE)
	{
		return;
	}

	VR(vkWaitForFences(vkDevice, 1, &vkFence, VK_TRUE, INFINITE));
}

void SVkSwapChain::ResizeBuffers(const Vector2N& Size)
{
	for (size_t i = 0; i < SwapChainImages.size(); ++i)
	{
		auto* Image = SwapChainImages[i];
		if (Image)
		{
			Image->Dispose();
			Image = nullptr;
		}
	}

	VkSwapchainCreateInfoKHR vkSwapChainCreateInfo =
	{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = vkSurface,
		.minImageCount = (uint32_t)SwapChainImages.size(),
		.imageFormat = VK_FORMAT_B8G8R8A8_UNORM,
		.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
		.imageExtent = VkExtent2D{ (uint32_t)Size.X, (uint32_t)Size.Y },
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices = {},
		.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR,
		.clipped = VK_TRUE,
		.oldSwapchain = vkSwapChain
	};

	VR(vkCreateSwapchainKHR(vkDevice, &vkSwapChainCreateInfo, nullptr, &vkSwapChain));

	std::vector<VkImage> vkSwapChainImages(SwapChainImages.size());
	auto vkSwapChainCount = (uint32_t)SwapChainImages.size();
	VR(vkGetSwapchainImagesKHR(vkDevice, vkSwapChain, &vkSwapChainCount, vkSwapChainImages.data()));

	for (size_t i = 0; i < SwapChainImages.size(); ++i)
	{
		if (SwapChainImages[i] == nullptr)
		{
			SwapChainImages[i] = gcnew SVkImage(Cast<SVkDevice>(GetDevice()), vkSwapChainImages[i], Size, false);
		}
		else
		{
			SwapChainImages[i]->vkImage = vkSwapChainImages[i];
		}
	}

	// Transit image layout to present mode.
	VkFence vkFence = ReadyFence();

	VR(vkResetCommandPool(vkDevice, vkCommandPool, 0));
	VkCommandBufferAllocateInfo vkCommandBufferCreateInfo =
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = vkCommandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};

	VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;
	VR(vkAllocateCommandBuffers(vkDevice, &vkCommandBufferCreateInfo, &vkCommandBuffer));

	std::vector<VkImageMemoryBarrier> vkBarriers;
	vkBarriers.reserve(SwapChainImages.size());

	for (size_t i = 0; i < SwapChainImages.size(); ++i)
	{
		vkBarriers.emplace_back() =
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			.image = SwapChainImages[i]->vkImage,
			.subresourceRange =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.levelCount = 1,
				.layerCount = 1
			}
		};
	}

	VkCommandBufferBeginInfo vkCommandBufferBegin =
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
	};

	VR(vkBeginCommandBuffer(vkCommandBuffer, &vkCommandBufferBegin));
	vkCmdPipelineBarrier(vkCommandBuffer,
		VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
		VK_DEPENDENCY_BY_REGION_BIT,
		0, nullptr,
		0, nullptr,
		(uint32_t)vkBarriers.size(), vkBarriers.data());
	VR(vkEndCommandBuffer(vkCommandBuffer));

	VkSubmitInfo vkSubmit =
	{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount = 1,
		.pCommandBuffers = &vkCommandBuffer,
	};

	VR(vkQueueSubmit(vkQueue, 1, &vkSubmit, vkFence));
}

IRHITexture2D* SVkSwapChain::GetBuffer(int32 Index)
{
	return SwapChainImages[Index];
}

VkFence SVkSwapChain::ReadyFence()
{
	if (vkFence == VK_NULL_HANDLE)
	{
		VkFenceCreateInfo vkFenceCreate =
		{
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
		};

		VR(vkCreateFence(vkDevice, &vkFenceCreate, nullptr, &vkFence));
	}
	else
	{
		VR(vkWaitForFences(vkDevice, 1, &vkFence, VK_TRUE, INFINITE));
		VR(vkResetFences(vkDevice, 1, &vkFence));
	}

	return vkFence;
}

void SVkSwapChain::Dispose(bool bDisposing)
{
	if (vkCommandPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(vkDevice, vkCommandPool, nullptr);
		vkCommandPool = VK_NULL_HANDLE;
	}

	if (vkFence != VK_NULL_HANDLE)
	{
		vkDestroyFence(vkDevice, vkFence, nullptr);
		vkFence = VK_NULL_HANDLE;
	}

	if (vkSwapChain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(vkDevice, vkSwapChain, nullptr);
		vkSwapChain = VK_NULL_HANDLE;
	}

	if (vkSurface != VK_NULL_HANDLE)
	{
		vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);
		vkSurface = VK_NULL_HANDLE;
	}

	Super::Dispose(bDisposing);
}