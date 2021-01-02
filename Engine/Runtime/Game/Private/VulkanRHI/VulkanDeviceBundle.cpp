// Copyright 2020 Aumoa.lib. All right reserved.

#include "VulkanDeviceBundle.h"

#include "Logging/LogMacros.h"
#include "RHI/IRHISwapChain.h"
#include "RHI/IRHIImmediateCommandList.h"
#include "RHI/IRHICommandFence.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIResource.h"
#include "RHI/IRHIDeferredCommandList.h"
#include "Application.h"
#include "VulkanSwapChain.h"

using namespace std;

VulkanDeviceBundle::VulkanDeviceBundle() : Super()
	, vkInstance(VK_NULL_HANDLE)
	, vkPhysicalDevice(VK_NULL_HANDLE)
	, vkDebugReportCallback(VK_NULL_HANDLE)
	, vkQueueIndex(0)
	
	, vkDeviceFeatures{ }
	, vkDevice(VK_NULL_HANDLE)
	, vkSurface(VK_NULL_HANDLE)
{

}

VulkanDeviceBundle::~VulkanDeviceBundle()
{

}

void VulkanDeviceBundle::InitializeBundle()
{
	uint32 extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	SE_LOG(LogVulkanRHI, Verbose, L"Vulkan RHI supports {0} extensions.", extensionCount);

	set<string> vkFullExts = GetFullExtensions();
	set<string> vkExtensions;
	vector<const char*> vkLayersName;
	vector<const char*> vkExtensionsName;
#ifdef _DEBUG
	vkLayersName.push_back("VK_LAYER_KHRONOS_validation");
#endif

	CheckLayers(vkLayersName.data(), vkLayersName.size(), vkExtensions, vkFullExts);
	AddExtension(vkExtensions, vkFullExts, "VK_EXT_debug_report");
	AddExtension(vkExtensions, vkFullExts, "VK_KHR_win32_surface");
	AddExtension(vkExtensions, vkFullExts, "VK_KHR_surface");

	vkExtensionsName.resize(vkExtensions.size());
	{
		auto assign_it = vkExtensionsName.begin();
		for (auto it = vkExtensions.begin(); it != vkExtensions.end(); ++it)
		{
			*assign_it++ = it->c_str();
		}
	}

	SE_LOG(LogVulkanRHI, Verbose, L"Enabled vulkan layers: \n{0}", String::Join(L"\n", vkLayersName));
	SE_LOG(LogVulkanRHI, Verbose, L"Enabled vulkan extensions: \n{0}", String::Join(L"\n", vkExtensions));

	VkApplicationInfo vkAppInfo = { };
	vkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vkAppInfo.pApplicationName = "GameApp";
	vkAppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	vkAppInfo.pEngineName = "Smart Engine";
	vkAppInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	vkAppInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo vkCreateInfo = { };
	vkCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkCreateInfo.pApplicationInfo = &vkAppInfo;
	vkCreateInfo.enabledLayerCount = (uint32)vkLayersName.size();
	vkCreateInfo.ppEnabledLayerNames = vkLayersName.data();
	vkCreateInfo.enabledExtensionCount = (uint32)vkExtensionsName.size();
	vkCreateInfo.ppEnabledExtensionNames = vkExtensionsName.data();
	VKR(vkCreateInstance(&vkCreateInfo, nullptr, &vkInstance));

	CreateDebugReportCallbackEXT();
	CreatePhysicalDevice(vkCreateInfo);

	GApplication.Sizing += bind_delegate(Application_OnSizing);
}

void VulkanDeviceBundle::ReleaseBundle()
{
	swapChain.Reset();

	DestroySurfaceKHR();
	DestroyDebugReportCallbackEXT();
	
	if (vkDevice != VK_NULL_HANDLE)
	{
		vkDestroyDevice(vkDevice, nullptr);
		vkDevice = VK_NULL_HANDLE;
	}

	if (vkInstance != VK_NULL_HANDLE)
	{
		vkDestroyInstance(vkInstance, nullptr);
		vkInstance = VK_NULL_HANDLE;
	}
}

TRefPtr<IRHISwapChain> VulkanDeviceBundle::GetSwapChain() const
{
	// TODO: NOTIMPL
	throw;
}

TRefPtr<IRHIImmediateCommandList> VulkanDeviceBundle::GetImmediateCommandList() const
{
	// TODO: NOTIMPL
	throw;
}

TRefPtr<IRHICommandFence> VulkanDeviceBundle::CreateCommandFence()
{
	// TODO: NOTIMPL
	throw;
}

TRefPtr<IRHIRenderTargetView> VulkanDeviceBundle::CreateRenderTargetView(IRHIResource* resource)
{
	// TODO: NOTIMPL
	throw;
}

TRefPtr<IRHIResource> VulkanDeviceBundle::CreateTexture2D(ERHITextureFormat format, int32 width, int32 height, ERHIResourceStates initialStates, ERHIResourceFlags flags)
{
	// TODO: NOTIMPL
	throw;
}

TRefPtr<IRHIDeferredCommandList> VulkanDeviceBundle::CreateDeferredCommandList()
{
	// TODO: NOTIMPL
	throw;
}

set<string> VulkanDeviceBundle::GetFullExtensions()
{
	set<string> exts;

	uint32 numSupports = 0;
	VKR(vkEnumerateInstanceExtensionProperties(nullptr, &numSupports, nullptr));

	vector<VkExtensionProperties> supportedExts((size_t)numSupports);
	VKR(vkEnumerateInstanceExtensionProperties(nullptr, &numSupports, supportedExts.data()));

	for (size_t i = 0; i < supportedExts.size(); ++i)
	{
		exts.emplace(supportedExts[i].extensionName);
	}

	return move(exts);
}

void VulkanDeviceBundle::CheckLayers(const char** layers, size_t numLayers, set<string>& extensions, const set<string>& fullExts)
{
	if (numLayers == 0)
	{
		return;
	}

	uint32 vkNumLayers = 0;
	VKR(vkEnumerateInstanceLayerProperties(&vkNumLayers, nullptr));

	vector<VkLayerProperties> vkLayers((size_t)vkNumLayers);
	VKR(vkEnumerateInstanceLayerProperties(&vkNumLayers, vkLayers.data()));

	for (size_t i = 0; i < numLayers; ++i)
	{
		//
		// Check layer supported by vulkan instance.
		bool bPass = false;
		for (size_t j = 0; j < vkLayers.size(); ++j)
		{
			if (strcmp(layers[i], vkLayers[j].layerName) == 0)
			{
				bPass = true;
				break;
			}
		}

		if (!bPass)
		{
			throw VulkanException(VK_ERROR_LAYER_NOT_PRESENT);
		}

		//
		// Check extensions that requires by layer.
		uint32 propertyCount = 0;
		VKR(vkEnumerateInstanceExtensionProperties(layers[i], &propertyCount, nullptr));

		vector<VkExtensionProperties> properties((size_t)propertyCount);
		VKR(vkEnumerateInstanceExtensionProperties(layers[i], &propertyCount, properties.data()));

		for (size_t j = 0; j < properties.size(); ++j)
		{
			if (!fullExts.contains(properties[j].extensionName))
			{
				SE_LOG(LogVulkanRHI, Warning, "Vulkan instance not support extension name: {0}", properties[j].extensionName);
			}
			else
			{
				extensions.emplace(properties[j].extensionName);
			}
		}
	}
}

void VulkanDeviceBundle::AddExtension(set<string>& extensions, const set<string>& fullExts, const string& item_add)
{
	if (fullExts.contains(item_add))
	{
		extensions.emplace(item_add);
	}
	else
	{
		throw VulkanException(VK_ERROR_EXTENSION_NOT_PRESENT);
	}
}

void VulkanDeviceBundle::CreateDebugReportCallbackEXT()
{
#ifdef _DEBUG
	VkDebugReportCallbackCreateInfoEXT vkCreateInfo = { };
	vkCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	vkCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	vkCreateInfo.pfnCallback = &DebugCallback;

	auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(vkInstance, "vkCreateDebugReportCallbackEXT");
	if (vkCreateDebugReportCallbackEXT == nullptr)
	{
		throw VulkanException(VK_ERROR_EXTENSION_NOT_PRESENT);
	}

	VKR(vkCreateDebugReportCallbackEXT(vkInstance, &vkCreateInfo, nullptr, &vkDebugReportCallback));
#endif
}

void VulkanDeviceBundle::DestroyDebugReportCallbackEXT()
{
	if (vkInstance == VK_NULL_HANDLE)
	{
		return;
	}

	if (vkDebugReportCallback == VK_NULL_HANDLE)
	{
		return;
	}

	auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(vkInstance, "vkDestroyDebugReportCallbackEXT");
	if (vkDestroyDebugReportCallbackEXT == nullptr)
	{
		SE_LOG(LogVulkanRHI, Warning, L"There is not address for destroy debug report callback.");
		return;
	}

	vkDestroyDebugReportCallbackEXT(vkInstance, vkDebugReportCallback, nullptr);
}

void VulkanDeviceBundle::CreatePhysicalDevice(const VkInstanceCreateInfo& vkInstanceCreateInfo)
{
	auto vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(vkInstance, "vkCreateWin32SurfaceKHR");
	if (vkCreateWin32SurfaceKHR == nullptr)
	{
		throw VulkanException(VK_ERROR_FEATURE_NOT_PRESENT);
	}

	VkWin32SurfaceCreateInfoKHR vkCreateInfo = { };
	vkCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	vkCreateInfo.hwnd = GApplication.GetCoreHwnd();
	vkCreateInfo.hinstance = GetModuleHandleW(nullptr);
	VKR(vkCreateWin32SurfaceKHR(vkInstance, &vkCreateInfo, nullptr, &vkSurface));

	uint32 deviceCount = 0;
	VKR(vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr));

	vector<VkPhysicalDevice> devices(deviceCount);
	VKR(vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data()));

	for (size_t i = 0; i < devices.size(); ++i)
	{
		optional<size_t> queue_index = CheckPhysicalDeviceFeature(devices[i]);
		if (queue_index.has_value())
		{
			vkPhysicalDevice = devices[i];
			vkQueueIndex = queue_index.value();
		}
	}

	if (vkPhysicalDevice == VK_NULL_HANDLE)
	{
		throw VulkanException(VK_ERROR_FEATURE_NOT_PRESENT);
	}

	float queuePriority = 1.0f;

	VkDeviceQueueCreateInfo vkQueueCreateInfo = { };
	vkQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	vkQueueCreateInfo.queueFamilyIndex = (uint32)vkQueueIndex;
	vkQueueCreateInfo.queueCount = 1;
	vkQueueCreateInfo.pQueuePriorities = &queuePriority;

	vector<const char*> vkExtensions;
	vkExtensions.push_back("VK_KHR_swapchain");

	VkDeviceCreateInfo vkDeviceCreateInfo = { };
	vkDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	vkDeviceCreateInfo.pQueueCreateInfos = &vkQueueCreateInfo;
	vkDeviceCreateInfo.queueCreateInfoCount = 1;
	vkDeviceCreateInfo.pEnabledFeatures = &vkDeviceFeatures;
	vkDeviceCreateInfo.enabledExtensionCount = (uint32)vkExtensions.size();
	vkDeviceCreateInfo.ppEnabledExtensionNames = vkExtensions.data();
	VKR(vkCreateDevice(vkPhysicalDevice, &vkDeviceCreateInfo, nullptr, &vkDevice));
	vkGetDeviceQueue(vkDevice, (uint32)vkQueueIndex, 0, &vkDirectQueue);
}

optional<size_t> VulkanDeviceBundle::CheckPhysicalDeviceFeature(VkPhysicalDevice physicalDevice)
{
	VkPhysicalDeviceProperties vkDeviceProps;
	vkGetPhysicalDeviceProperties(physicalDevice, &vkDeviceProps);

	// Is dedicated GPU.
	if (vkDeviceProps.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		return nullopt;
	}

	VkPhysicalDeviceFeatures vkDeviceFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &vkDeviceFeatures);

	// Is support all shader types.
	if (!vkDeviceFeatures.geometryShader || !vkDeviceFeatures.tessellationShader)
	{
		return nullopt;
	}

	//
	// Check device is support swap chain.
	uint32 availableExts;
	VKR(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &availableExts, nullptr));

	vector<VkExtensionProperties> vkAvailableExts((size_t)availableExts);
	VKR(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &availableExts, vkAvailableExts.data()));

	bool bPass = false;
	for (size_t i = 0; i < vkAvailableExts.size(); ++i)
	{
		if (vkAvailableExts[i].extensionName == "VK_KHR_swapchain"s)
		{
			bPass = true;
			break;
		}
	}

	if (!bPass)
	{
		return nullopt;
	}

	if (!CheckSupportSwapChainDetails(physicalDevice))
	{
		return nullopt;
	}

	//
	// Is support direct queue.
	uint32 queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	vector<VkQueueFamilyProperties> queueFamilies((size_t)queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	optional<size_t> queueIndex;
	for (size_t i = 0; i < queueFamilies.size(); ++i)
	{
		auto& family = queueFamilies[i];
		VkBool32 bPresentSupport = false;

		if (FAILED(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, (uint32)i, vkSurface, &bPresentSupport)))
		{
			continue;
		}

		if (bPresentSupport && family.queueCount > 0 && (family.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT)) != 0)
		{
			queueIndex = i;
			break;
		}
	}

	if (queueIndex.has_value())
	{
		this->vkDeviceFeatures = vkDeviceFeatures;
	}
	return move(queueIndex);
}

void VulkanDeviceBundle::DestroySurfaceKHR()
{
	if (vkInstance == VK_NULL_HANDLE)
	{
		return;
	}

	if (vkSurface == VK_NULL_HANDLE)
	{
		return;
	}

	auto vkDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)vkGetInstanceProcAddr(vkInstance, "vkDestroySurfaceKHR");
	if (vkDestroySurfaceKHR == nullptr)
	{
		SE_LOG(LogVulkanRHI, Warning, L"There is not address for destroy surface.");
		return;
	}

	vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);
}

bool VulkanDeviceBundle::CheckSupportSwapChainDetails(VkPhysicalDevice vkDevice)
{
	uint32 formatCount = 0;
	VKR(vkGetPhysicalDeviceSurfaceFormatsKHR(vkDevice, vkSurface, &formatCount, nullptr));

	if (formatCount == 0)
	{
		return false;
	}

	uint32 presentModeCount = 0;
	VKR(vkGetPhysicalDeviceSurfacePresentModesKHR(vkDevice, vkSurface, &presentModeCount, nullptr));
	if (presentModeCount == 0)
	{
		return false;
	}

	VkSurfaceCapabilitiesKHR vkCapabilities;
	VKR(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkDevice, vkSurface, &vkCapabilities));
	if (vkCapabilities.minImageCount > 2 || vkCapabilities.maxImageCount < 3)
	{
		return false;
	}

	vector<VkSurfaceFormatKHR> vkSurfaceFormats((size_t)formatCount);
	VKR(vkGetPhysicalDeviceSurfaceFormatsKHR(vkDevice, vkSurface, &formatCount, vkSurfaceFormats.data()));

	vector<VkPresentModeKHR> vkPresentModes((size_t)presentModeCount);
	VKR(vkGetPhysicalDeviceSurfacePresentModesKHR(vkDevice, vkSurface, &presentModeCount, vkPresentModes.data()));

	bool bFormatSuitable = false;
	bool bPresentModeSuitable = false;

	for (size_t i = 0; i < vkSurfaceFormats.size(); ++i)
	{
		if (vkSurfaceFormats[i].format == VK_FORMAT_B8G8R8A8_UNORM)
		{
			bFormatSuitable = true;
			break;
		}
	}

	for (size_t i = 0; i < vkPresentModes.size(); ++i)
	{
		if (vkPresentModes[i] == VK_PRESENT_MODE_FIFO_KHR)
		{
			bPresentModeSuitable = true;
			break;
		}
	}

	return bFormatSuitable && bPresentModeSuitable;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDeviceBundle::DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData)
{
	SE_LOG(LogVulkanRHI, Error, msg);

	return VK_FALSE;
}

void VulkanDeviceBundle::Application_OnSizing(int32 width, int32 height)
{
	if (swapChain.IsValid)
	{
		swapChain->ResizeBuffers(width, height);
	}
	else
	{
		swapChain = NewObject<VulkanSwapChain>(vkDevice, vkSurface, width, height);
	}
}