// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkGraphics.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericWindowSwapchainExtension.h"
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

        VKR(vkCreateInstance(&vkInstanceCreateInfo, nullptr, m_Instance.ReleaseAndGetAddressOf()));

        uint32_t gpuCount = 0;
        VKR(vkEnumeratePhysicalDevices(m_Instance.Get(), &gpuCount, nullptr));

        std::vector<VkPhysicalDevice> physicalDevices{ gpuCount };
        VKR(vkEnumeratePhysicalDevices(m_Instance.Get(), &gpuCount, physicalDevices.data()));

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
        vkGetDeviceQueue(m_Device, 0, 0, &m_GraphicsQueue);
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

        if (m_Device != nullptr)
        {
            vkDestroyDevice(m_Device, nullptr);
            m_Device = nullptr;
        }
    }

    void VkGraphics::InstallSwapChain(std::shared_ptr<GenericWindow> targetWindow)
    {
        class SwapChainExtension : public GenericWindowSwapchainExtension
        {
        private:
            VkGraphics* m_Owner;
            VkSurfaceKHR m_Surface;
            VkSwapchainKHR m_Swapchain;

        public:
            SwapChainExtension(VkGraphics* owner, VkSurfaceKHR surface, VkSwapchainKHR swapchain)
                : m_Owner(owner)
                , m_Surface(surface)
                , m_Swapchain(swapchain)
            {
            }

            virtual ~SwapChainExtension() noexcept override
            {
                vkDestroySurfaceKHR(m_Owner->m_Instance.Get(), m_Surface, nullptr);
            }

            virtual void Present() override
            {
                uint32_t imageIndex;
                VKR(vkAcquireNextImageKHR(m_Owner->m_Device, m_Swapchain, UINT64_MAX, m_Owner->m_Semaphore, VK_NULL_HANDLE, &imageIndex));

                VkPresentInfoKHR presentInfo
                {
                    .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                    .waitSemaphoreCount = 1,
                    .pWaitSemaphores = &m_Owner->m_Semaphore,
                    .swapchainCount = 1,
                    .pSwapchains = &m_Swapchain,
                    .pImageIndices = &imageIndex
                };
                VKR(vkQueuePresentKHR(m_Owner->m_GraphicsQueue, &presentInfo));
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
        VKR(vkCreateXlibSurfaceKHR(m_Instance.Get(), &surfaceInfo, nullptr, &surface));
        
        VkSurfaceCapabilitiesKHR caps;
        VKR(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, surface, &caps));

        uint32_t formatCount = 0;
        VKR(vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, surface, &formatCount, nullptr));

        std::vector<VkSurfaceFormatKHR> formats{ (size_t)formatCount };
        VKR(vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, surface, &formatCount, formats.data()));

        size_t chosenFormatIndex = -1;
        for (size_t i = 0; i < formats.size(); ++i)
        {
            PlatformProcess::OutputDebugString(String::Format(TEXT("{}"), formats[i].format));
            if (formats[i].format == VK_FORMAT_B8G8R8A8_UNORM)
            {
                chosenFormatIndex = i;
                break;
            }
        }

        if (chosenFormatIndex == -1){
            throw new InvalidOperationException(TEXT("Required format(B8G8R8A8_UNORM) not supported."));
        }

        VkSwapchainCreateInfoKHR swapchainCreateInfo
        {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface = surface,
            .minImageCount = 2,
            .imageFormat = formats[chosenFormatIndex].format,
            .imageColorSpace = formats[chosenFormatIndex].colorSpace,
            .imageExtent = caps.currentExtent,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .preTransform = caps.currentTransform,
            .presentMode = VK_PRESENT_MODE_FIFO_KHR,
            .clipped = VK_TRUE
        };

        VkSwapchainKHR swapchain;
        VKR(vkCreateSwapchainKHR(m_Device, &swapchainCreateInfo, nullptr, &swapchain));

        targetWindow->AddExtension(std::make_shared<SwapChainExtension>(this, surface, swapchain));
    }
}

DEFINE_CREATE_GRAPHICS(::Ayla::VkGraphics);