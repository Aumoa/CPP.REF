// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIDeviceBundle.h"

#include "VulkanMinimal.h"
#include <set>
#include <optional>

namespace SC::Runtime::Game::VulkanRHI
{
	class VulkanSwapChain;

	class VulkanDeviceBundle : virtual public Core::Object, virtual public RHI::IRHIDeviceBundle
	{
	public:
		using Super = Core::Object;
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

		Core::TRefPtr<VulkanSwapChain> swapChain;

	public:
		VulkanDeviceBundle();
		~VulkanDeviceBundle() override;

		virtual void InitializeBundle();
		virtual void ReleaseBundle();

		virtual Core::TRefPtr<RHI::IRHISwapChain> GetSwapChain() const;
		virtual Core::TRefPtr<RHI::IRHIImmediateCommandList> GetImmediateCommandList() const;

		virtual Core::TRefPtr<RHI::IRHICommandFence> CreateCommandFence();
		virtual Core::TRefPtr<RHI::IRHIRenderTargetView> CreateRenderTargetView(RHI::IRHIResource* resource);
		virtual Core::TRefPtr<RHI::IRHIResource> CreateTexture2D(RHI::RHITextureFormat format, int32 width, int32 height, RHI::RHIResourceStates initialStates, RHI::RHIResourceFlags flags);
		virtual Core::TRefPtr<RHI::IRHIDeferredCommandList> CreateDeferredCommandList();

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
}