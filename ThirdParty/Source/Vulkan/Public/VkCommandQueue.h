// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkDeviceChild.h"

class VULKAN_API SVkCommandQueue : public SVkDeviceChild, implements IRHICommandQueue
{
	GENERATED_BODY(SVkCommandQueue)

private:
	VkDevice vkDevice = VK_NULL_HANDLE;

public:
	VkQueue vkQueue = VK_NULL_HANDLE;

public:
	SVkCommandQueue(SVkDevice* Owner, VkQueue vkQueue);

	using Super::Dispose;

	virtual int32 AcquireSwapChainImage(IRHISwapChain* SwapChain) override;
	virtual void Present(IRHISwapChain* SwapChain, int32 BufferIndex) override;
	virtual void Submit(std::span<IRHICommandBuffer* const> CommandBuffers, IRHIFence* Fence) override;
};