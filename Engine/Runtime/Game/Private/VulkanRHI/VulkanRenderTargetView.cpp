// Copyright 2020 Aumoa.lib. All right reserved.

#include "VulkanRenderTargetView.h"

VulkanRenderTargetView::VulkanRenderTargetView(VkDevice vkDevice, VkImageView vkImageView) : Super()
	, vkDevice(vkDevice)
	, vkImageView(vkImageView)
{

}

VulkanRenderTargetView::~VulkanRenderTargetView()
{
	if (vkDevice == VK_NULL_HANDLE)
	{
		return;
	}

	if (vkImageView == VK_NULL_HANDLE)
	{
		return;
	}

	vkDestroyImageView(vkDevice, vkImageView, nullptr);
}