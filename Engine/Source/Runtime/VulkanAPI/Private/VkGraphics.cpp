// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkGraphics.h"
#include "GenericApplication.h"
#include "GenericWindowSwapchainExtension.h"
#include "VkSwapchainExt.h"
#include "Linq/Concat.h"
#include <ranges>

namespace Ayla
{
    VkGraphics::VkGraphics()
    {
        auto& app = GenericApplication::Get();

        VkApplicationInfo vkAppInfo =
        {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "No named",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "AylaEngine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_2
        };

        uint32_t extCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
        std::vector<VkExtensionProperties> supportedExtensions(extCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, supportedExtensions.data());

        LogVulkan::Verbose(TEXT("Support instance extensions:"));

        for (size_t i = 0; i < supportedExtensions.size(); ++i)
        {
            auto& extension = supportedExtensions[i];
            LogVulkan::Verbose(TEXT("  #{}: {}"), i, String::FromLiteral(extension.extensionName));
        }

        static constexpr std::array<const char*, 1> kLayers
        {
			"VK_LAYER_KHRONOS_validation"
        };

        static constexpr std::array<const char*, 2> kExtensions
        {
            "VK_EXT_debug_utils",
            "VK_KHR_get_physical_device_properties2"
        };

        std::vector<const char*> extensions =
            std::ranges::to<std::vector<const char*>>(
                app.GetVulkanExtensionNames() | Linq::Concat(kExtensions)
            );
        
        VkInstanceCreateInfo vkInstanceCreateInfo =
        {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &vkAppInfo,
			.enabledLayerCount = (uint32_t)kLayers.size(),
			.ppEnabledLayerNames = kLayers.data(),
            .enabledExtensionCount = (uint32_t)extensions.size(),
            .ppEnabledExtensionNames = extensions.data()
        };

        VKR(vkCreateInstance(&vkInstanceCreateInfo, nullptr, &m_Instance));

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo =
        {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = [](VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) -> VkBool32
            {
                LogVulkan::Verbose(String::Format(TEXT("Vulkan: {}"), String::FromCodepage(pCallbackData->pMessage)));
                return VK_FALSE;
            }
		};

        uint32_t gpuCount = 0;
        VKR(vkEnumeratePhysicalDevices(m_Instance, &gpuCount, nullptr));

        std::vector<VkPhysicalDevice> physicalDevices{ gpuCount };
        VKR(vkEnumeratePhysicalDevices(m_Instance, &gpuCount, physicalDevices.data()));

        auto formatDeviceType = [](VkPhysicalDeviceType dt)
        {
            switch (dt)
            {
                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    return TEXT("INTEGRATED_GPU");
                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    return TEXT("DISCRETE_GPU");
                case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                    return TEXT("VIRTUAL_GPU");
                case VK_PHYSICAL_DEVICE_TYPE_CPU:
                    return TEXT("CPU");
            }

            return TEXT("UNKNOWN");
        };

        std::vector<VkPhysicalDeviceProperties> physicalDeviceProps;
        physicalDeviceProps.reserve(physicalDevices.size());
        for (size_t i = 0; i < physicalDevices.size(); ++i)
        {
            auto& pd = physicalDevices[i];
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(pd, &props);
            physicalDeviceProps.emplace_back(props);
            LogVulkan::Verbose(TEXT("Physical Device #{}: {} ({})"), i, String::FromLiteral(props.deviceName), formatDeviceType(props.deviceType));

            uint32_t extensionsCount = 0;
            VKR(vkEnumerateDeviceExtensionProperties(pd, nullptr, &extensionsCount, nullptr));

            std::vector<VkExtensionProperties> pdExtensions{ (size_t)extensionsCount };
            VKR(vkEnumerateDeviceExtensionProperties(pd, nullptr, &extensionsCount, pdExtensions.data()));

            LogVulkan::Verbose(TEXT("  Extensions: "));
            for (auto& extension : pdExtensions)
            {
                LogVulkan::Verbose(TEXT("    {}"), String::FromCodepage(extension.extensionName));
            }
        }

        // Find a queue family that supports VK_QUEUE_GRAPHICS_BIT
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &m_QueueCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(m_QueueCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &m_QueueCount, queueFamilies.data());

        m_GraphicsQueueFamilyIndex = (uint32_t)-1;
        for (uint32_t i = 0; i < m_QueueCount; ++i)
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                m_GraphicsQueueFamilyIndex = i;
                break;
            }
        }
        if (m_GraphicsQueueFamilyIndex == -1)
        {
            throw std::runtime_error("No queue family supports VK_QUEUE_GRAPHICS_BIT");
        }

        const float queuePriorities[] = { 1.0f };
        std::vector<VkDeviceQueueCreateInfo> vkQueueInfos;
		vkQueueInfos.reserve((size_t)m_QueueCount);
        for (uint32_t i = 0; i < m_QueueCount; ++i)
        {
            vkQueueInfos.emplace_back(VkDeviceQueueCreateInfo
            {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = i,
                .queueCount = 1,
                .pQueuePriorities = queuePriorities
            });
        }

        VkPhysicalDeviceTimelineSemaphoreFeatures timelineFeatures =
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES,
            .timelineSemaphore = VK_TRUE
        };

        // Enable buffer device address feature for raytracing support
        VkPhysicalDeviceBufferDeviceAddressFeatures bufferAddressFeatures =
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES,
            .pNext = nullptr,
            .bufferDeviceAddress = VK_TRUE,
            .bufferDeviceAddressCaptureReplay = VK_FALSE,
            .bufferDeviceAddressMultiDevice = VK_FALSE
        };

        // Enable descriptor indexing features (required by VK_KHR_acceleration_structure)
        VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures =
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES,
            .pNext = nullptr,
            .runtimeDescriptorArray = VK_TRUE
        };

        // Enable raytracing pipeline features
        VkPhysicalDeviceRayTracingPipelineFeaturesKHR raytracingPipelineFeatures =
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR,
            .pNext = nullptr,
            .rayTracingPipeline = VK_TRUE
        };

        // Enable acceleration structure features
        VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeatures =
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR,
            .pNext = nullptr,
            .accelerationStructure = VK_TRUE
        };

        // Chain features: timelineFeatures -> bufferAddressFeatures -> descriptorIndexingFeatures -> raytracingPipelineFeatures -> accelerationStructureFeatures
        timelineFeatures.pNext = &bufferAddressFeatures;
        bufferAddressFeatures.pNext = &descriptorIndexingFeatures;
        descriptorIndexingFeatures.pNext = &raytracingPipelineFeatures;
        raytracingPipelineFeatures.pNext = &accelerationStructureFeatures;

        std::vector<const char*> deviceExtensions = 
        {
            "VK_KHR_swapchain",
            // Core extensions for Vulkan 1.2 features when using extension names
            "VK_KHR_buffer_device_address",
            "VK_EXT_descriptor_indexing",
            "VK_KHR_spirv_1_4",
            "VK_KHR_shader_float_controls",
            // Raytracing extensions
            "VK_KHR_acceleration_structure",
            "VK_KHR_ray_tracing_pipeline",
            "VK_KHR_deferred_host_operations"
        };

        VkDeviceCreateInfo vkDeviceInfo =
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = &timelineFeatures,
             .queueCreateInfoCount = (uint32_t)vkQueueInfos.size(),
             .pQueueCreateInfos = vkQueueInfos.data(),
             .enabledExtensionCount = (uint32_t)deviceExtensions.size(),
             .ppEnabledExtensionNames = deviceExtensions.data()
         };

        LogVulkan::Verbose(TEXT("Logical device created using {} physical device."), String::FromLiteral(physicalDeviceProps[0].deviceName));
        VKR(vkCreateDevice(physicalDevices[0], &vkDeviceInfo, nullptr, &m_Device));
        vkGetDeviceQueue(m_Device, m_GraphicsQueueFamilyIndex, 0, &m_GraphicsQueue);
        m_PhysicalDevice = physicalDevices[0];
    }

    VkGraphics::~VkGraphics() noexcept
    {
        Dispose();
    }

    void VkGraphics::Dispose() noexcept
    {
        m_GraphicsQueue = nullptr;
        m_PhysicalDevice = nullptr;
    }

    SharedPtr<GenericWindowSwapchainExtension> VkGraphics::InstallSwapChain_Implementation(SharedPtr<GenericWindow> targetWindow)
    {
#if PLATFORM_LINUX
        auto* display = reinterpret_cast<Display*>(GenericApplication::Get().GetApplicationPointer());
        auto window = reinterpret_cast<Window>(targetWindow->GetOSWindowHandle());

        VkXlibSurfaceCreateInfoKHR surfaceInfo
        {
            .sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
            .dpy = display,
            .window = window
        };

        VkSurfaceKHR surface;
        VKR(vkCreateXlibSurfaceKHR(m_Instance, &surfaceInfo, nullptr, &surface));
#elif PLATFORM_WINDOWS
        auto* hwnd = reinterpret_cast<HWND>(targetWindow->GetOSWindowHandle());
        VkWin32SurfaceCreateInfoKHR surfaceInfo
        {
            .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .hinstance = GetModuleHandle(nullptr),
            .hwnd = hwnd
        };
        VkSurfaceKHR surface;
		VKR(vkCreateWin32SurfaceKHR(m_Instance, &surfaceInfo, nullptr, &surface));
#endif
        
        VkSurfaceCapabilitiesKHR caps;
        VKR(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, surface, &caps));

        uint32_t formatCount = 0;
        VKR(vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, surface, &formatCount, nullptr));

        std::vector<VkSurfaceFormatKHR> formats{ (size_t)formatCount };
        VKR(vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, surface, &formatCount, formats.data()));

        size_t chosenFormatIndex = (size_t)-1;
        for (size_t i = 0; i < formats.size(); ++i)
        {
            if (formats[i].format == VK_FORMAT_B8G8R8A8_UNORM)
            {
                chosenFormatIndex = i;
                break;
            }
        }

        if (chosenFormatIndex == (size_t)-1)
        {
            throw new InvalidOperationException(TEXT("Required format(B8G8R8A8_UNORM) not supported."));
        }

        VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        if (caps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
        {
            compositeAlpha = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
        }

        uint32_t imageCount = (uint32_t)kMaxSwapchainImages;
		check(caps.maxImageCount == 0 || (size_t)caps.maxImageCount >= kMaxSwapchainImages);
		check(caps.minImageCount == 0 || (size_t)caps.minImageCount <= kMaxSwapchainImages);

        VkSwapchainCreateInfoKHR swapchainCreateInfo
        {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface = surface,
            .minImageCount = imageCount,
            .imageFormat = formats[chosenFormatIndex].format,
            .imageColorSpace = formats[chosenFormatIndex].colorSpace,
            .imageExtent = caps.currentExtent,
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .preTransform = caps.currentTransform,
            .compositeAlpha = compositeAlpha,
            .presentMode = VK_PRESENT_MODE_FIFO_KHR,
            .clipped = VK_TRUE
        };

        VkQueue suitableQueue = VK_NULL_HANDLE;
        for (uint32_t i = 0; i < m_QueueCount; ++i)
        {
            VkBool32 supported;
            VKR(vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, surface, &supported));
            if (supported)
            {
                vkGetDeviceQueue(m_Device, i, 0, &suitableQueue);
            }
        }

        if (suitableQueue == VK_NULL_HANDLE)
        {
			throw new InvalidOperationException(TEXT("No suitable queue found for swapchain."));
        }

        VkSwapchainKHR swapchain;
        VKR(vkCreateSwapchainKHR(m_Device, &swapchainCreateInfo, nullptr, &swapchain));

        auto extension = New<VkSwapchainExt>(this, surface, swapchain, swapchainCreateInfo, suitableQueue);
        targetWindow->AddExtension(extension);
        return extension;
    }

    void VkGraphics::BeginRenderFrame()
    {
        static thread_local std::vector<function_t<void()>> s_Continuations;
        static thread_local std::vector<decltype(m_FenceCompletionCallbacks)::iterator> s_RemoveIterators;

        std::unique_lock lock(m_FenceCompletionMutex);
		for (auto it = m_FenceCompletionCallbacks.begin(); it != m_FenceCompletionCallbacks.end(); ++it)
        {
            VkResult result = vkGetFenceStatus(m_Device, it->first);
            if (result == VK_SUCCESS)
            {
                s_Continuations.emplace_back(std::move(it->second));
                s_RemoveIterators.emplace_back(std::move(it));
            }
        }

        for (auto& it : s_RemoveIterators)
        {
            m_FenceCompletionCallbacks.erase(it);
		}
        s_RemoveIterators.clear();

        lock.unlock();

        for (auto& continuation : s_Continuations)
        {
            continuation();
        }
        s_Continuations.clear();
    }

    void VkGraphics::EndRenderFrame()
    {
        ++m_FrameCount;
    }

    void VkGraphics::WaitForCompletion()
    {
        vkDeviceWaitIdle(m_Device);
    }

    PFN_vkSetDebugUtilsObjectNameEXT VkGraphics::GetSetDebugUtilsObjectNameEXTFunction() const noexcept
    {
        return reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(vkGetDeviceProcAddr(m_Device, "vkSetDebugUtilsObjectNameEXT"));
    }
    
    void VkGraphics::AddFenceCompletionCallback(VkFence fence, function_t<void()> continuation)
    {
        std::unique_lock lock(m_FenceCompletionMutex);
        m_FenceCompletionCallbacks.emplace(fence, std::move(continuation));
    }
}