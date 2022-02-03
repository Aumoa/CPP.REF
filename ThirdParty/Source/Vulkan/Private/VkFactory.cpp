// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "VkFactory.h"
#include "VkAdapter.h"
#include "VkDevice.h"
#include "VkSwapChain.h"
#include "VkCommandQueue.h"
#include "IApplicationInterface.h"
#include "Exceptions/InvalidArgumentException.h"

GENERATE_BODY(SVkFactory);

#define VkGetProc(ProcName) ((PFN_ ## ProcName)vkGetInstanceProcAddr(vkInstance, #ProcName))
#define VkInvoke(ProcName, ...) VkGetProc(ProcName)(__VA_ARGS__)

SVkFactory::SVkFactory(bool bEnableDebugLayer) : Super()
{
	uint32_t vkLayerCount = 0;
	VR(vkEnumerateInstanceLayerProperties(&vkLayerCount, nullptr));

	std::vector<VkLayerProperties> vkLayers((size_t)vkLayerCount);
	if (vkLayerCount)
	{
		VR(vkEnumerateInstanceLayerProperties(&vkLayerCount, vkLayers.data()));
		for (auto& Layer : vkLayers)
		{
			SE_LOG(LogVulkan, Verbose, L"Find vulkan layer: {}: {}", ANSI_TO_WCHAR(Layer.layerName), ANSI_TO_WCHAR(Layer.description));
		}
	}

	uint32_t vkExtensionsCount = 0;
	VR(vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionsCount, nullptr));

	std::vector<VkExtensionProperties> vkExtensions((size_t)vkExtensionsCount);
	if (vkExtensionsCount)
	{
		VR(vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionsCount, vkExtensions.data()));
		for (auto& Extension : vkExtensions)
		{
			SE_LOG(LogVulkan, Verbose, L"Find vulkan extension: {}", ANSI_TO_WCHAR(Extension.extensionName));
		}
	}

	std::vector<const char*> vkRequiredLayers =
	{
	};
	std::vector<const char*> vkOptionalLayers =
	{
		"VK_LAYER_KHRONOS_validation"
	};
	std::vector<const char*> vkEnabledLayers = VkEnableLayers(vkLayers, vkRequiredLayers, vkOptionalLayers);

	std::vector<const char*> vkRequiredExtensions =
	{
		VK_KHR_SURFACE_EXTENSION_NAME,

#if PLATFORM_WINDOWS
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
	};
	std::vector<const char*> vkOptionalExtensions =
	{
	};

	if (bEnableDebugLayer)
	{
		vkOptionalExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	std::vector<const char*> vkEnabledExtensions = VkEnableExtensions(vkExtensions, vkRequiredExtensions, vkOptionalExtensions);

	VkApplicationInfo vkAppInfo =
	{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = SE_APPLICATION,
		.applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 1),
		.pEngineName = "CPP.REF",
		.engineVersion = VK_MAKE_API_VERSION(0, 0, 0, 1),
		.apiVersion = VK_API_VERSION_1_2
	};

	VkInstanceCreateInfo vkCreateInfo =
	{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &vkAppInfo,
		.enabledLayerCount = (uint32_t)vkEnabledLayers.size(),
		.ppEnabledLayerNames = vkEnabledLayers.data(),
		.enabledExtensionCount = (uint32_t)vkEnabledExtensions.size(),
		.ppEnabledExtensionNames = vkEnabledExtensions.data()
	};

	VR(vkCreateInstance(&vkCreateInfo, nullptr, &vkInstance));
	SE_LOG(LogVulkan, Verbose, L"Vulkan device created with {} layers and {} extensions.", vkCreateInfo.enabledLayerCount, vkCreateInfo.enabledExtensionCount);
	
	if (bEnableDebugLayer)
	{
		VkDebugUtilsMessengerCreateInfoEXT vkDebugCreateInfo =
		{
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
			.pfnUserCallback = &SVkFactory::ValidationLayerCallback,
			.pUserData = this
		};

		VkInvoke(vkCreateDebugUtilsMessengerEXT, vkInstance, &vkDebugCreateInfo, nullptr, &vkDebugCallback);
	}

	uint32_t vkPhysicalDevicesCount = 0;
	VR(vkEnumeratePhysicalDevices(vkInstance, &vkPhysicalDevicesCount, nullptr));
	
	std::vector<VkPhysicalDevice> vkPhysicalDevices((size_t)vkPhysicalDevicesCount);
	VR(vkEnumeratePhysicalDevices(vkInstance, &vkPhysicalDevicesCount, vkPhysicalDevices.data()));

	for (auto& vkPhysicalDevice : vkPhysicalDevices)
	{
		PhysicalDevices.emplace_back(gcnew SVkAdapter(this, std::move(vkPhysicalDevice)));
	}
}

void SVkFactory::Dispose()
{
	this->Dispose(true);
	GC.SuppressFinalize(this);
}

IRHIAdapter* SVkFactory::GetAdapter(int32 Index)
{
	if (Index < 0 || Index >= (int32)PhysicalDevices.size())
	{
		SE_LOG(LogVulkan, Error, L"Index out of range. DesiredIndex: {}, NumPhysicalDevices: {}", Index, PhysicalDevices.size());
		return nullptr;
	}

	return PhysicalDevices[Index];
}

IRHIDevice* SVkFactory::CreateDevice(IRHIAdapter* Adapter)
{
	VkPhysicalDevice vkPhysDev = Cast<SVkAdapter>(Adapter)->GetPhysicalDevice();

	uint32_t vkLayerCount = 0;
	VR(vkEnumerateDeviceLayerProperties(vkPhysDev, &vkLayerCount, nullptr));

	std::vector<VkLayerProperties> vkLayers((size_t)vkLayerCount);
	if (vkLayerCount)
	{
		VR(vkEnumerateDeviceLayerProperties(vkPhysDev, &vkLayerCount, vkLayers.data()));
		for (auto& Layer : vkLayers)
		{
			SE_LOG(LogVulkan, Verbose, L"Find vulkan layer for physical device({}): {}: {}", Adapter->GetDeviceName(), ANSI_TO_WCHAR(Layer.layerName), ANSI_TO_WCHAR(Layer.description));
		}
	}

	uint32_t vkExtensionsCount = 0;
	VR(vkEnumerateDeviceExtensionProperties(vkPhysDev, nullptr, &vkExtensionsCount, nullptr));

	std::vector<VkExtensionProperties> vkExtensions((size_t)vkExtensionsCount);
	if (vkExtensionsCount)
	{
		VR(vkEnumerateDeviceExtensionProperties(vkPhysDev,nullptr, &vkExtensionsCount, vkExtensions.data()));
		for (auto& Extension : vkExtensions)
		{
			SE_LOG(LogVulkan, Verbose, L"Find vulkan extension for physical device({}): {}", Adapter->GetDeviceName(), ANSI_TO_WCHAR(Extension.extensionName));
		}
	}

	std::vector<const char*> vkRequiredLayers =
	{
	};
	std::vector<const char*> vkOptionalLayers =
	{
	};
	std::vector<const char*> vkEnabledLayers = VkEnableLayers(vkLayers, vkRequiredLayers, vkOptionalLayers);

	std::vector<const char*> vkRequiredExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	};
	std::vector<const char*> vkOptionalExtensions =
	{
	};

	std::vector<const char*> vkEnabledExtensions = VkEnableExtensions(vkExtensions, vkRequiredExtensions, vkOptionalExtensions);

	VkPhysicalDeviceFeatures vkFeatures;
	vkGetPhysicalDeviceFeatures(vkPhysDev, &vkFeatures);

	VkDeviceQueueCreateInfo vkQueueCreateInfo =
	{
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = 0,
		.queueCount = 1
	};

	VkDeviceCreateInfo vkDeviceCreateInfo =
	{
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &vkQueueCreateInfo,
		.enabledLayerCount = (uint32_t)vkEnabledLayers.size(),
		.ppEnabledLayerNames = vkEnabledLayers.data(),
		.enabledExtensionCount = (uint32_t)vkEnabledExtensions.size(),
		.ppEnabledExtensionNames = vkEnabledExtensions.data(),
		.pEnabledFeatures = &vkFeatures
	};

	VkDevice vkDevice = VK_NULL_HANDLE;
	VR(vkCreateDevice(vkPhysDev, &vkDeviceCreateInfo, nullptr, &vkDevice));

	return gcnew SVkDevice(this, std::move(vkDevice));
}

IRHISwapChain* SVkFactory::CreateSwapChain(IRHICommandQueue* Queue, size_t NumBuffers)
{
	VkSurfaceKHR vkSurface = VK_NULL_HANDLE;

#if PLATFORM_WINDOWS
	VkWin32SurfaceCreateInfoKHR vkSurfaceCreateInfo =
	{
		.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
		.hinstance = GetModuleHandleW(nullptr),
		.hwnd = (HWND)IApplicationInterface::Get().GetWindowHandle()
	};

	VR(vkCreateWin32SurfaceKHR(vkInstance, &vkSurfaceCreateInfo, nullptr, &vkSurface));
#endif

	auto sQueue = Cast<SVkCommandQueue>(Queue);
	return gcnew SVkSwapChain(Cast<SVkDevice>(Queue->GetDevice()), vkSurface, NumBuffers, sQueue->vkQueue);
}

void SVkFactory::Dispose(bool bDisposing)
{
	VkInvoke(vkDestroyDebugUtilsMessengerEXT, vkInstance, vkDebugCallback, nullptr);
	vkDestroyInstance(vkInstance, nullptr);
}

std::vector<const char*> SVkFactory::VkEnableLayers(std::vector<VkLayerProperties>& Layers, const std::vector<const char*>& Requires, const std::vector<const char*>& Optionals)
{
	std::map<std::string_view, size_t, std::less<>> LayersIndex;
	std::vector<bool> Referenced(Layers.size());
	for (size_t i = 0; i < Layers.size(); ++i)
	{
		LayersIndex.emplace(Layers[i].layerName, i);
	}

	for (auto& LayerName : Requires)
	{
		auto It = LayersIndex.find(LayerName);
		VkCheckSlow(It != LayersIndex.end(), VK_ERROR_LAYER_NOT_PRESENT);
		Referenced[It->second] = true;
	}

	for (auto& LayerName : Optionals)
	{
		auto It = LayersIndex.find(LayerName);
		if (It != LayersIndex.end())
		{
			Referenced[It->second] = true;
		}
	}

	// Remove unreferenced layer properties.
	for (size_t i = Referenced.size(); i > 0; --i)
	{
		if (!Referenced[i - 1])
		{
			Layers.erase(Layers.begin() + i - 1);
		}
	}

	std::vector<const char*> LayerNames(Layers.size());
	std::transform(Layers.begin(), Layers.end(), LayerNames.begin(), [](VkLayerProperties& LayerProp)
	{
		return LayerProp.layerName;
	});

	return LayerNames;
}

std::vector<const char*> SVkFactory::VkEnableExtensions(std::vector<VkExtensionProperties>& Extensions, const std::vector<const char*>& Requires, const std::vector<const char*>& Optionals)
{
	std::map<std::string_view, size_t, std::less<>> ExtensionsIndex;
	std::vector<bool> Referenced(Extensions.size());
	for (size_t i = 0; i < Extensions.size(); ++i)
	{
		ExtensionsIndex.emplace(Extensions[i].extensionName, i);
	}

	for (auto& LayerName : Requires)
	{
		auto It = ExtensionsIndex.find(LayerName);
		VkCheckSlow(It != ExtensionsIndex.end(), VK_ERROR_LAYER_NOT_PRESENT);
		Referenced[It->second] = true;
	}

	for (auto& LayerName : Optionals)
	{
		auto It = ExtensionsIndex.find(LayerName);
		if (It != ExtensionsIndex.end())
		{
			Referenced[It->second] = true;
		}
	}

	// Remove unreferenced layer properties.
	for (size_t i = Referenced.size(); i > 0; --i)
	{
		if (!Referenced[i - 1])
		{
			Extensions.erase(Extensions.begin() + i - 1);
		}
	}

	std::vector<const char*> LayerNames(Extensions.size());
	std::transform(Extensions.begin(), Extensions.end(), LayerNames.begin(), [](VkExtensionProperties& LayerProp)
	{
		return LayerProp.extensionName;
	});

	return LayerNames;
}

VKAPI_ATTR VkBool32 VKAPI_CALL SVkFactory::ValidationLayerCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT Severity,
	VkDebugUtilsMessageTypeFlagsEXT Type,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	ELogVerbosity Verbosity;

	switch (Severity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		Verbosity = ELogVerbosity::Verbose;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		Verbosity = ELogVerbosity::Warning;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		Verbosity = ELogVerbosity::Error;
		break;
	default:
		throw gcnew SInvalidArgumentException(L"Internal error. Severity is not valid from callback by vulkan validation layer.");
	}

	LogSystem::Log(LogVulkan, Verbosity, L"{}", ANSI_TO_WCHAR(pCallbackData->pMessage));
	return VK_FALSE;
}

#undef VkGetProc
#undef VkInvoke