// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkGraphics.h"
#include "GenericApplication.h"
#include "GenericWindowSwapchainExtension.h"
#include "VkSwapchainExt.h"
#include "Linq/Concat.h"
#include <ranges>

namespace Ayla
{
    VkGraphics::VkGraphics(GenericApplication* app)
    {
        VkApplicationInfo vkAppInfo =
        {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "No named",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "AylaEngine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_0
        };

        uint32_t extCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
        std::vector<VkExtensionProperties> supportedExtensions(extCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, supportedExtensions.data());

        PlatformProcess::OutputDebugString(TEXT("Support instance extensions:\n"));

        for (size_t i = 0; i < supportedExtensions.size(); ++i)
        {
            auto& extension = supportedExtensions[i];
            PlatformProcess::OutputDebugString(String::Format(TEXT("  #{}: {}\n"), i, String::FromLiteral(extension.extensionName)));
        }

        static constexpr std::array<const char*, 1> kLayers
        {
			"VK_LAYER_KHRONOS_validation"
        };

        static constexpr std::array<const char*, 1> kExtensions
        {
            "VK_EXT_debug_utils"
        };

        std::vector<const char*> extensions =
            std::ranges::to<std::vector<const char*>>(
                app->GetVulkanExtensionNames() | Linq::Concat(kExtensions)
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
                PlatformProcess::OutputDebugString(String::Format(TEXT("Vulkan: {}"), String::FromCodepage(pCallbackData->pMessage)));
                return VK_FALSE;
            }
		};

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
        if (func)
        {
        }

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

        for (size_t i = 0; i < physicalDevices.size(); ++i)
        {
            auto& pd = physicalDevices[i];
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(pd, &props);
            PlatformProcess::OutputDebugString(String::Format(TEXT("Physical Device #{}: {} ({})"), i, String::FromLiteral(props.deviceName), formatDeviceType(props.deviceType)));

            uint32_t extensionsCount = 0;
            VKR(vkEnumerateDeviceExtensionProperties(pd, nullptr, &extensionsCount, nullptr));

            std::vector<VkExtensionProperties> pdExtensions{ (size_t)extensionsCount };
            VKR(vkEnumerateDeviceExtensionProperties(pd, nullptr, &extensionsCount, pdExtensions.data()));

            PlatformProcess::OutputDebugString(TEXT("  Extensions: "));
            for (auto& extension : pdExtensions)
            {
                PlatformProcess::OutputDebugString(String::Format(TEXT("    {}"), String::FromCodepage(extension.extensionName)));
            }
        }


        // Find a queue family that supports VK_QUEUE_GRAPHICS_BIT
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &queueFamilyCount, queueFamilies.data());

        int graphicsQueueFamilyIndex = -1;
        for (uint32_t i = 0; i < queueFamilyCount; ++i)
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                graphicsQueueFamilyIndex = i;
                break;
            }
        }
        if (graphicsQueueFamilyIndex == -1)
        {
            throw std::runtime_error("No queue family supports VK_QUEUE_GRAPHICS_BIT");
        }

        const float queuePriorities[] = { 1.0f };
        VkDeviceQueueCreateInfo vkQueueInfos[] =
        {
            {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = (uint32_t)graphicsQueueFamilyIndex,
                .queueCount = 1,
                .pQueuePriorities = queuePriorities
            }
        };

        std::vector<const char*> deviceExtensions = { "VK_KHR_swapchain" };
        VkDeviceCreateInfo vkDeviceInfo =
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = vkQueueInfos,
            .enabledExtensionCount = (uint32_t)deviceExtensions.size(),
            .ppEnabledExtensionNames = deviceExtensions.data()
        };

        VKR(vkCreateDevice(physicalDevices[0], &vkDeviceInfo, nullptr, &m_Device));
        vkGetDeviceQueue(m_Device, graphicsQueueFamilyIndex, 0, &m_GraphicsQueue);
        m_PhysicalDevice = physicalDevices[0];

        VkSemaphoreCreateInfo semaphoreCreateInfo
        {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
        };
        VKR(vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_Semaphore));
    }

    VkGraphics::~VkGraphics() noexcept
    {
        m_GraphicsQueue = nullptr;
        m_PhysicalDevice = nullptr;

        if (m_Semaphore)
        {
            vkDestroySemaphore(m_Device, m_Semaphore, nullptr);
            m_Semaphore = nullptr;
        }
    }

    void VkGraphics::InstallSwapChain(std::shared_ptr<GenericWindow> targetWindow)
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
            PlatformProcess::OutputDebugString(String::Format(TEXT("{}"), formats[i].format));
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

        VkSwapchainCreateInfoKHR swapchainCreateInfo
        {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface = surface,
            .minImageCount = 2,
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

        VkSwapchainKHR swapchain;
        VKR(vkCreateSwapchainKHR(m_Device, &swapchainCreateInfo, nullptr, &swapchain));

        targetWindow->AddExtension(std::make_shared<VkSwapchainExt>(this, surface, swapchain, swapchainCreateInfo));
    }
}

DEFINE_CREATE_GRAPHICS(::Ayla::VkGraphics);