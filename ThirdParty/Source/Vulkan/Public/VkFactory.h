// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIInterfaces.h"
#include "VkCommon.h"

class SVkAdapter;

class VULKAN_API SVkFactory : implements SObject, implements IRHIFactory
{
	GENERATED_BODY(SVkFactory)

private:
	VkInstance vkInstance = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT vkDebugCallback = VK_NULL_HANDLE;

	SPROPERTY(PhysicalDevices)
	std::vector<SVkAdapter*> PhysicalDevices;

public:
	SVkFactory(bool bEnableDebugLayer = false);

	virtual void Dispose() override;

	virtual IRHIAdapter* GetAdapter(int32 Index) override;
	virtual IRHIDevice* CreateDevice(IRHIAdapter* Adapter) override;
	virtual IRHISwapChain* CreateSwapChain(IRHICommandQueue* Queue, size_t NumBuffers) override;

	VkInstance GetVkInstance() { return vkInstance; }

protected:
	virtual void Dispose(bool bDisposing) override;

private:
	static std::vector<const char*> VkEnableLayers(std::vector<VkLayerProperties>& Layers, const std::vector<const char*>& Requires, const std::vector<const char*>& Optionals);
	static std::vector<const char*> VkEnableExtensions(std::vector<VkExtensionProperties>& Extensions, const std::vector<const char*>& Requires, const std::vector<const char*>& Optionals);

	static VKAPI_ATTR VkBool32 VKAPI_CALL ValidationLayerCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT Severity,
		VkDebugUtilsMessageTypeFlagsEXT Type,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
};