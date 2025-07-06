// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkGraphics.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericWindowExtension.h"
#include "GenericPlatform/GenericApplication.h"

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
        
        auto extensions = app->GetVulkanExtensionNames();
        VkInstanceCreateInfo vkInstanceCreateInfo =
        {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &vkAppInfo,
            .enabledExtensionCount = (uint32_t)extensions.size(),
            .ppEnabledExtensionNames = extensions.data()
        };

        VKR(vkCreateInstance(&vkInstanceCreateInfo, nullptr, &m_Instance));

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
            vkEnumerateDeviceExtensionProperties(pd, nullptr, &extensionsCount, nullptr);

            std::vector<VkExtensionProperties> pdExtensions{ (size_t)extensionsCount };
            vkEnumerateDeviceExtensionProperties(pd, nullptr, &extensionsCount, pdExtensions.data());

            PlatformProcess::OutputDebugString(TEXT("  Extensions: "));
            for (auto& extension : pdExtensions)
            {
                PlatformProcess::OutputDebugString(String::Format(TEXT("    {}"), String::FromCodepage(extension.extensionName)));
            }
        }

        const float queuePriorities[] = { 1.0f };
        VkDeviceQueueCreateInfo vkQueueInfos[] =
        {
            {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT,
                .queueCount = 1,
                .pQueuePriorities = queuePriorities
            }
        };

        std::vector<const char*> deviceExtensions = { "VK_KHR_swapchain" };
        VkDeviceCreateInfo vkDeviceInfo =
        {.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = vkQueueInfos,
            .enabledExtensionCount = (uint32_t)deviceExtensions.size(),
            .ppEnabledExtensionNames = deviceExtensions.data()
        };

        VKR(vkCreateDevice(physicalDevices[0], &vkDeviceInfo, nullptr, &m_Device));
        vkGetDeviceQueue(m_Device, VK_QUEUE_GRAPHICS_BIT, 0, &m_GraphicsQueue);
    }

    void VkGraphics::InstallSwapChain(std::shared_ptr<GenericWindow> targetWindow)
    {
        class SwapChainExtension : public GenericWindowExtension
        {
            VkInstance m_Instance;
            VkSurfaceKHR m_Surface;

        public:
            SwapChainExtension(VkInstance instance, VkSurfaceKHR surface)
                : m_Instance(instance)
                , m_Surface(surface)
            {
            }

            virtual ~SwapChainExtension() noexcept override
            {
                vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
            }
        };

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

        targetWindow->AddExtension(std::make_shared<SwapChainExtension>(m_Instance, surface));
    }
}

DEFINE_CREATE_GRAPHICS(::Ayla::VkGraphics);