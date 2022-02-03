// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkFactoryChild.h"

class VULKAN_API SVkDevice : public SVkFactoryChild, implements IRHIDevice
{
	GENERATED_BODY(SVkDevice)

public:
	VkDevice vkDevice = VK_NULL_HANDLE;
	VkQueue vkQueue = VK_NULL_HANDLE;

public:
	SVkDevice(SVkFactory* Owner, VkDevice vkDevice);

	using Super::Dispose;

	virtual IRHICommandQueue* CreateCommandQueue() override;
	virtual IRHITexture2D* CreateTexture2D(const RHITexture2DDesc& InDesc, const RHISubresourceData* InitialData) override { throw; }
	virtual IRHIRenderTargetView* CreateRenderTargetView(int32 Count) override;
	virtual IRHIFence* CreateFence() override;
	virtual IRHICommandAllocator* CreateCommandAllocator() override;
	virtual IRHICommandBuffer* CreateCommandBuffer(IRHICommandAllocator* Allocator) override;
	virtual IRHIRenderPass* CreateRenderPass() override;

protected:
	virtual void Dispose(bool bDisposing) override;
};