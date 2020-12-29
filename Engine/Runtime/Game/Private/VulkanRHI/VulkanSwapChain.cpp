// Copyright 2020 Aumoa.lib. All right reserved.

#include "VulkanSwapChain.h"

#include "VulkanResource.h"

using namespace std;

VulkanSwapChain::VulkanSwapChain(VkDevice vkDevice, VkSurfaceKHR vkSurface, int32 width, int32 height) : Super()
	, vkDevice(VK_NULL_HANDLE)
	, vkSwapChain(VK_NULL_HANDLE)

	, vkCreateInfo{ }
	, vkSurface(VK_NULL_HANDLE)
	, bufferCount(3)
{
	vkCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	vkCreateInfo.surface = vkSurface;
	vkCreateInfo.minImageCount = 3;
	vkCreateInfo.imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
	vkCreateInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	vkCreateInfo.imageExtent = VkExtent2D{ (uint32)width, (uint32)height };
	vkCreateInfo.imageArrayLayers = 1;
	vkCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	vkCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vkCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	vkCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	vkCreateInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
	vkCreateInfo.oldSwapchain = vkSwapChain;
	ReallocSwapChain();

	this->vkDevice = vkDevice;
	this->vkSurface = vkSurface;
}

VulkanSwapChain::~VulkanSwapChain()
{
	if (vkDevice == VK_NULL_HANDLE)
	{
		return;
	}

	if (vkSwapChain == VK_NULL_HANDLE)
	{
		return;
	}

	vkDestroySwapchainKHR(vkDevice, vkSwapChain, nullptr);
}

void VulkanSwapChain::Present()
{
	// TODO: NOTIMPL
	throw;
}

void VulkanSwapChain::ResizeBuffers(int32 width, int32 height)
{
	vkCreateInfo.minImageCount = 3;
	vkCreateInfo.imageExtent = VkExtent2D{ (uint32)width, (uint32)height };
	vkCreateInfo.oldSwapchain = vkSwapChain;
	ReallocSwapChain();
}

TRefPtr<IRHIResource> VulkanSwapChain::GetBuffer(size_t index) const
{
	// TODO: NOTIMPL
	throw;
}

size_t VulkanSwapChain::CurrentBackBufferIndex_get() const
{
	// TODO: NOTIMPL
	throw;
}

size_t VulkanSwapChain::NumBuffers_get() const
{
	return bufferCount;
}

void VulkanSwapChain::ReallocSwapChain()
{
	VKR(vkCreateSwapchainKHR(vkDevice, &vkCreateInfo, nullptr, &vkSwapChain));

	uint32 imageCount = 0;
	VKR(vkGetSwapchainImagesKHR(vkDevice, vkSwapChain, &imageCount, nullptr));

	vector<VkImage> vkImages((size_t)imageCount);
	VKR(vkGetSwapchainImagesKHR(vkDevice, vkSwapChain, &imageCount, vkImages.data()));

	buffers.resize((size_t)imageCount);
	for (size_t i = 0; i < buffers.size(); ++i)
	{
		buffers[i] = NewObject<VulkanResource>(vkImages[i], false);
	}
	bufferCount = buffers.size();
}