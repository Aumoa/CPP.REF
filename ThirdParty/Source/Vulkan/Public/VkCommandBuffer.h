// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkDeviceChild.h"

class VULKAN_API SVkCommandBuffer : public SVkDeviceChild, implements IRHICommandBuffer
{
	GENERATED_BODY(SVkCommandBuffer)

public:
	VkDevice vkDevice = VK_NULL_HANDLE;
	VkCommandPool vkCommandPool = VK_NULL_HANDLE;
	VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;

public:
	SVkCommandBuffer(SVkDevice* Owner, VkCommandPool vkCommandPool, VkCommandBuffer vkCommandBuffer);

	using Super::Dispose;

	virtual void BeginRecord() override;
	virtual void EndRecord() override;
	virtual void ResourceBarrier(std::span<const RHIResourceBarrier> InBarriers) override;
	virtual void ClearRenderTargetView(IRHIRenderTargetView* RTV, int32 IndexOf, const Color& InColor) override;
	virtual void BeginRenderPass(IRHIRenderPass* RenderPass, const RectN& ScissorRect, std::span<Color const> InColor) override;
	virtual void EndRenderPass() override;
};