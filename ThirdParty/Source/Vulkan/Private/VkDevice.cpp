// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "VkDevice.h"
#include "VkCommandQueue.h"
#include "VkFence.h"
#include "VkCommandAllocator.h"
#include "VkCommandBuffer.h"
#include "VkImageView.h"
#include "VkRenderPass.h"

GENERATE_BODY(SVkDevice);

SVkDevice::SVkDevice(SVkFactory* Owner, VkDevice vkDevice)
	: Super(Owner)
	, vkDevice(std::move(vkDevice))
{
	vkGetDeviceQueue(vkDevice, 0, 0, &vkQueue);
}

IRHICommandQueue* SVkDevice::CreateCommandQueue()
{
	return gcnew SVkCommandQueue(this, vkQueue);
}

IRHIRenderTargetView* SVkDevice::CreateRenderTargetView(int32 Count)
{
	return gcnew SVkImageView(this, (size_t)Count);
}

IRHIFence* SVkDevice::CreateFence()
{
	VkFenceCreateInfo vkFenceCreateInfo =
	{
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
	};

	VkFence vkFence = VK_NULL_HANDLE;
	VR(vkCreateFence(vkDevice, &vkFenceCreateInfo, nullptr, &vkFence));
	
	return gcnew SVkFence(this, std::move(vkFence));
}

IRHICommandAllocator* SVkDevice::CreateCommandAllocator()
{
	VkCommandPoolCreateInfo vkPoolCreateInfo =
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = 0,
	};

	VkCommandPool vkPool = VK_NULL_HANDLE;
	VR(vkCreateCommandPool(vkDevice, &vkPoolCreateInfo, nullptr, &vkPool));

	return gcnew SVkCommandAllocator(this, std::move(vkPool));
}

IRHICommandBuffer* SVkDevice::CreateCommandBuffer(IRHICommandAllocator* Allocator)
{
	VkCommandBufferAllocateInfo vkAllocateInfo =
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = Cast<SVkCommandAllocator>(Allocator)->vkCommandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};

	VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;
	VR(vkAllocateCommandBuffers(vkDevice, &vkAllocateInfo, &vkCommandBuffer));

	return gcnew SVkCommandBuffer(this, std::move(vkAllocateInfo.commandPool), std::move(vkCommandBuffer));
}

IRHIRenderPass* SVkDevice::CreateRenderPass()
{
	return gcnew SVkRenderPass(this);
}

void SVkDevice::Dispose(bool bDisposing)
{
	if (vkDevice != VK_NULL_HANDLE)
	{
		vkDestroyDevice(vkDevice, nullptr);
		vkDevice = nullptr;
	}

	Super::Dispose(bDisposing);
}