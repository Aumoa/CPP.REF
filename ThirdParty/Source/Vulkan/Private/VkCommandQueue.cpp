// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "VkCommandQueue.h"
#include "VkSwapChain.h"
#include "VkFence.h"
#include "VkDevice.h"
#include "VkCommandBuffer.h"

GENERATE_BODY(SVkCommandQueue);

SVkCommandQueue::SVkCommandQueue(SVkDevice* Owner, VkQueue vkQueue)
	: Super(Owner)
	, vkQueue(std::move(vkQueue))
{
	vkDevice = Owner->vkDevice;
}

int32 SVkCommandQueue::AcquireSwapChainImage(IRHISwapChain* SwapChain)
{
	auto* sSwapChain = Cast<SVkSwapChain>(SwapChain);
	VkSwapchainKHR vkSwapChain = sSwapChain->vkSwapChain;
	VkFence vkFence = sSwapChain->ReadyFence();

	uint32_t vkImageIndex = 0;
	VR(vkAcquireNextImageKHR(vkDevice, vkSwapChain, std::numeric_limits<uint64_t>::max(), VK_NULL_HANDLE, vkFence, &vkImageIndex));

	return (int32)vkImageIndex;
}

void SVkCommandQueue::Present(IRHISwapChain* SwapChain, int32 BufferIndex)
{
	VkSwapchainKHR vkSwapChain = Cast<SVkSwapChain>(SwapChain)->vkSwapChain;

	VkPresentInfoKHR vkPresent =
	{
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.swapchainCount = 1,
		.pSwapchains = &vkSwapChain,
		.pImageIndices = (uint32_t*)&BufferIndex,
	};

	VkResult Result = vkQueuePresentKHR(vkQueue, &vkPresent);
	if (Result != VK_ERROR_OUT_OF_DATE_KHR)
	{
		VR(Result);
	}
}

void SVkCommandQueue::Submit(std::span<IRHICommandBuffer* const> CommandBuffers, IRHIFence* Fence)
{
	std::vector<VkCommandBuffer> vkCommandBuffers;
	vkCommandBuffers.reserve(CommandBuffers.size());

	for (auto& CommandBuffer : CommandBuffers)
	{
		auto sCommandBuffer = Cast<SVkCommandBuffer>(CommandBuffer);
		vkCommandBuffers.emplace_back(sCommandBuffer->vkCommandBuffer);
	}

	VkSubmitInfo vkSubmit =
	{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount = (uint32_t)vkCommandBuffers.size(),
		.pCommandBuffers = vkCommandBuffers.data()
	};

	VkFence vkFence = VK_NULL_HANDLE;
	if (auto sFence = Cast<SVkFence>(Fence); sFence)
	{
		vkFence = sFence->vkFence;
		VR(vkResetFences(vkDevice, 1, &vkFence));
	}

	VR(vkQueueSubmit(vkQueue, 1, &vkSubmit, vkFence));
}