// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIDeviceBundle.h"

#include "VulkanMinimal.h"
#include <set>
#include <optional>

class VulkanSwapChain;

class VulkanDeviceBundle : virtual public Object, virtual public IRHIDeviceBundle
{
public:
	using Super = Object;
	using This = VulkanDeviceBundle;

private:
	VkInstance vkInstance;
	VkPhysicalDevice vkPhysicalDevice;
	VkDebugReportCallbackEXT vkDebugReportCallback;
	size_t vkQueueIndex;

	VkPhysicalDeviceFeatures vkDeviceFeatures;
	VkDevice vkDevice;
	VkQueue vkDirectQueue;
	VkSurfaceKHR vkSurface;

	TRefPtr<VulkanSwapChain> swapChain;

public:
	VulkanDeviceBundle();
	~VulkanDeviceBundle() override;

	virtual void InitializeBundle();
	virtual void ReleaseBundle();

	virtual TRefPtr<IRHISwapChain> GetSwapChain() const;
	virtual TRefPtr<IRHIImmediateCommandList> GetImmediateCommandList() const;

	virtual TRefPtr<IRHICommandFence> CreateCommandFence();
	virtual TRefPtr<IRHIRenderTargetView> CreateRenderTargetView(IRHIResource* resource);
	virtual TRefPtr<IRHIResource> CreateTexture2D(RHITextureFormat format, int32 width, int32 height, RHIResourceStates initialStates, RHIResourceFlags flags);
	virtual TRefPtr<IRHIDeferredCommandList> CreateDeferredCommandList();

private:
	std::set<std::string> GetFullExtensions();
	void CheckLayers(const char** layers, size_t numLayers, std::set<std::string>& extensions, const std::set<std::string>& fullExts);
	void AddExtension(std::set<std::string>& extensions, const std::set<std::string>& fullExts, const std::string& item_add);
	void CreateDebugReportCallbackEXT();
	void DestroyDebugReportCallbackEXT();
	void CreatePhysicalDevice(const VkInstanceCreateInfo& vkInstanceCreateInfo);
	std::optional<size_t> CheckPhysicalDeviceFeature(VkPhysicalDevice physicalDevice);
	void DestroySurfaceKHR();
	bool CheckSupportSwapChainDetails(VkPhysicalDevice vkDevice);

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);

	// CALLBACK HANDLERS
	void Application_OnSizing(int32 width, int32 height);
};