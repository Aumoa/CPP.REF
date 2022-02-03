// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkDeviceChild.h"

class SVkImage;
class SVkCommandQueue;

class VULKAN_API SVkSwapChain : public SVkDeviceChild, implements IRHISwapChain
{
	GENERATED_BODY(SVkSwapChain)

public:
	VkInstance vkInstance = VK_NULL_HANDLE;
	VkDevice vkDevice = VK_NULL_HANDLE;
	VkSurfaceKHR vkSurface = VK_NULL_HANDLE;
	VkSwapchainKHR vkSwapChain = VK_NULL_HANDLE;

	SPROPERTY(SwapChainImages)
	std::vector<SVkImage*> SwapChainImages;
	std::vector<VkImageView> vkSwapChainImageViews;

private:
	VkQueue vkQueue = VK_NULL_HANDLE;
	VkCommandPool vkCommandPool = VK_NULL_HANDLE;
	VkFence vkFence = VK_NULL_HANDLE;

public:
	SVkSwapChain(SVkDevice* Owner, VkSurfaceKHR vkSurface, size_t NumBuffers, VkQueue vkQueue);

	using Super::Dispose;

	virtual bool IsReady() override;
	virtual void Wait() override;

	virtual void ResizeBuffers(const Vector2N& Size) override;
	virtual IRHITexture2D* GetBuffer(int32 Index) override;

	VkFence ReadyFence();

protected:
	virtual void Dispose(bool bDisposing) override;

private:
	void VkImagesReady();
};