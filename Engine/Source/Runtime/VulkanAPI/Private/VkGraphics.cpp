// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkGraphics.h"
#include "GenericApplication.h"
#include "GenericWindowSwapchainExtension.h"
#include "VkSwapchainExt.h"
#include "VkCommandQueue.h"
#include "VkShader.h"
#include "VkGeometryRenderPipeline.h"
#include "VkRaytracingRenderPipeline.h"
#include "Linq/Concat.h"
#include <algorithm>
#include <ranges>
#include <array>
#include <cstring>

namespace Ayla
{
    namespace
    {
        static constexpr std::array<const char*, 8> kRequiredDeviceExtensions
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
            VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
            VK_KHR_SPIRV_1_4_EXTENSION_NAME,
            VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME,
            VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
            VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
            VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME
        };

        constexpr VkFormat kRequiredSwapchainFormat = VK_FORMAT_B8G8R8A8_UNORM;
        constexpr VkFormat kRequiredRaytracingOutputFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
        constexpr VkImageUsageFlags kRequiredSwapchainUsage =
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
            VK_IMAGE_USAGE_TRANSFER_DST_BIT;

        bool HasDeviceExtension(const std::vector<VkExtensionProperties>& extensions, const char* requiredExtensionName)
        {
            return std::ranges::any_of(extensions, [requiredExtensionName](const VkExtensionProperties& extension)
            {
                return std::strcmp(extension.extensionName, requiredExtensionName) == 0;
            });
        }

        void ThrowRequiredExtensionMissing(const char* deviceName, const char* requiredExtensionName)
        {
            throw InvalidOperationException(String::Format(
                TEXT("Vulkan physical device '{}' does not support required device extension '{}'."),
                String::FromCodepage(deviceName),
                String::FromLiteral(requiredExtensionName)
            ));
        }

        void ThrowRequiredFeatureMissing(const char* deviceName, String featureName)
        {
            throw InvalidOperationException(String::Format(
                TEXT("Vulkan physical device '{}' does not support required raytracing feature '{}'."),
                String::FromCodepage(deviceName),
                featureName
            ));
        }

        void ValidateRequiredDeviceExtensions(VkPhysicalDevice physicalDevice, const char* deviceName)
        {
            uint32_t extensionsCount = 0;
            VKR(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, nullptr));

            std::vector<VkExtensionProperties> extensions{ (size_t)extensionsCount };
            VKR(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, extensions.data()));

            for (const char* requiredExtensionName : kRequiredDeviceExtensions)
            {
                if (HasDeviceExtension(extensions, requiredExtensionName) == false)
                {
                    ThrowRequiredExtensionMissing(deviceName, requiredExtensionName);
                }
            }
        }

        void ValidateRequiredRaytracingFeatures(VkPhysicalDevice physicalDevice, const char* deviceName)
        {
            VkPhysicalDeviceTimelineSemaphoreFeatures timelineFeatures =
            {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES
            };

            VkPhysicalDeviceBufferDeviceAddressFeatures bufferAddressFeatures =
            {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES
            };

            VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures =
            {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES
            };

            VkPhysicalDeviceRayTracingPipelineFeaturesKHR raytracingPipelineFeatures =
            {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR
            };

            VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeatures =
            {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR
            };

            timelineFeatures.pNext = &bufferAddressFeatures;
            bufferAddressFeatures.pNext = &descriptorIndexingFeatures;
            descriptorIndexingFeatures.pNext = &raytracingPipelineFeatures;
            raytracingPipelineFeatures.pNext = &accelerationStructureFeatures;

            VkPhysicalDeviceFeatures2 features =
            {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
                .pNext = &timelineFeatures
            };

            vkGetPhysicalDeviceFeatures2(physicalDevice, &features);

            if (timelineFeatures.timelineSemaphore == VK_FALSE)
            {
                ThrowRequiredFeatureMissing(deviceName, TEXT("timelineSemaphore"));
            }

            if (bufferAddressFeatures.bufferDeviceAddress == VK_FALSE)
            {
                ThrowRequiredFeatureMissing(deviceName, TEXT("bufferDeviceAddress"));
            }

            if (descriptorIndexingFeatures.runtimeDescriptorArray == VK_FALSE)
            {
                ThrowRequiredFeatureMissing(deviceName, TEXT("runtimeDescriptorArray"));
            }

            if (raytracingPipelineFeatures.rayTracingPipeline == VK_FALSE)
            {
                ThrowRequiredFeatureMissing(deviceName, TEXT("rayTracingPipeline"));
            }

            if (accelerationStructureFeatures.accelerationStructure == VK_FALSE)
            {
                ThrowRequiredFeatureMissing(deviceName, TEXT("accelerationStructure"));
            }
        }

        template<class TFunction>
        TFunction LoadRequiredDeviceFunction(VkDevice device, const char* functionName)
        {
            auto function = reinterpret_cast<TFunction>(vkGetDeviceProcAddr(device, functionName));
            if (function == nullptr)
            {
                throw InvalidOperationException(String::Format(
                    TEXT("Required Vulkan device function '{}' is not available."),
                    String::FromLiteral(functionName)
                ));
            }

            return function;
        }

        struct QueueFamilyInfo
        {
            uint32_t FamilyIndex;
            int32 SupportsCount;
            VkQueueFlags Flags;
        };

        constexpr uint32_t kInvalidQueueFamilyIndex = (uint32_t)-1;

        QueueFamilyInfo MakeInvalidQueueFamilyInfo()
        {
            return QueueFamilyInfo{ kInvalidQueueFamilyIndex, -1, 0 };
        }

        String FormatDeviceType(VkPhysicalDeviceType deviceType)
        {
            switch (deviceType)
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
        }

        int32 ScorePhysicalDevice(const VkPhysicalDeviceProperties& properties)
        {
            switch (properties.deviceType)
            {
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                return 400;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                return 300;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                return 200;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                return 100;
            default:
                return 0;
            }
        }

        bool SupportsRequiredDeviceExtensions(VkPhysicalDevice physicalDevice)
        {
            uint32_t extensionsCount = 0;
            VKR(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, nullptr));

            std::vector<VkExtensionProperties> extensions{ (size_t)extensionsCount };
            VKR(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, extensions.data()));

            for (const char* requiredExtensionName : kRequiredDeviceExtensions)
            {
                if (HasDeviceExtension(extensions, requiredExtensionName) == false)
                {
                    return false;
                }
            }

            return true;
        }

        bool SupportsRequiredRaytracingFeatures(VkPhysicalDevice physicalDevice)
        {
            VkPhysicalDeviceTimelineSemaphoreFeatures timelineFeatures =
            {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES
            };

            VkPhysicalDeviceBufferDeviceAddressFeatures bufferAddressFeatures =
            {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES
            };

            VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures =
            {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES
            };

            VkPhysicalDeviceRayTracingPipelineFeaturesKHR raytracingPipelineFeatures =
            {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR
            };

            VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeatures =
            {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR
            };

            timelineFeatures.pNext = &bufferAddressFeatures;
            bufferAddressFeatures.pNext = &descriptorIndexingFeatures;
            descriptorIndexingFeatures.pNext = &raytracingPipelineFeatures;
            raytracingPipelineFeatures.pNext = &accelerationStructureFeatures;

            VkPhysicalDeviceFeatures2 features =
            {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
                .pNext = &timelineFeatures
            };

            vkGetPhysicalDeviceFeatures2(physicalDevice, &features);

            return timelineFeatures.timelineSemaphore != VK_FALSE
                && bufferAddressFeatures.bufferDeviceAddress != VK_FALSE
                && descriptorIndexingFeatures.runtimeDescriptorArray != VK_FALSE
                && raytracingPipelineFeatures.rayTracingPipeline != VK_FALSE
                && accelerationStructureFeatures.accelerationStructure != VK_FALSE;
        }

        bool TryFindQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, std::array<QueueFamilyInfo, 3>& queueFamilyInfos)
        {
            uint32_t queueCount;
            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, nullptr);
            std::vector<VkQueueFamilyProperties> queueFamilies((size_t)queueCount);
            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, queueFamilies.data());

            queueFamilyInfos =
            {
                MakeInvalidQueueFamilyInfo(),
                MakeInvalidQueueFamilyInfo(),
                MakeInvalidQueueFamilyInfo()
            };

            QueueFamilyInfo& bestPrimaryQueueFamily = queueFamilyInfos[0];
            QueueFamilyInfo& bestComputeQueueFamily = queueFamilyInfos[1];
            QueueFamilyInfo& bestTransferQueueFamily = queueFamilyInfos[2];

            for (uint32_t i = 0; i < queueCount; ++i)
            {
                VkBool32 supportsSurface = VK_FALSE;
                VKR(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &supportsSurface));

                bool supportsGraphics = (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0;
                bool supportsCompute = (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0;
                bool supportsTransfer = (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0;
                int32 supportsCount = (supportsGraphics ? 1 : 0) + (supportsCompute ? 1 : 0) + (supportsTransfer ? 1 : 0);

                if (supportsGraphics && supportsTransfer && supportsSurface)
                {
                    if (bestPrimaryQueueFamily.FamilyIndex == kInvalidQueueFamilyIndex || supportsCount > bestPrimaryQueueFamily.SupportsCount)
                    {
                        bestPrimaryQueueFamily = { i, supportsCount, queueFamilies[i].queueFlags };
                    }
                }

                if (supportsCompute)
                {
                    if (bestComputeQueueFamily.FamilyIndex == kInvalidQueueFamilyIndex || supportsCount < bestComputeQueueFamily.SupportsCount)
                    {
                        bestComputeQueueFamily = { i, supportsCount, queueFamilies[i].queueFlags };
                    }
                }

                if (supportsTransfer)
                {
                    if (bestTransferQueueFamily.FamilyIndex == kInvalidQueueFamilyIndex || supportsCount < bestTransferQueueFamily.SupportsCount)
                    {
                        bestTransferQueueFamily = { i, supportsCount, queueFamilies[i].queueFlags };
                    }
                }
            }

            return bestPrimaryQueueFamily.FamilyIndex != kInvalidQueueFamilyIndex
                && bestComputeQueueFamily.FamilyIndex != kInvalidQueueFamilyIndex
                && bestTransferQueueFamily.FamilyIndex != kInvalidQueueFamilyIndex;
        }

        bool TryChooseRequiredSwapchainFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceFormatKHR& chosenFormat)
        {
            uint32_t formatCount = 0;
            VKR(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr));

            std::vector<VkSurfaceFormatKHR> formats{ (size_t)formatCount };
            VKR(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data()));

            for (const VkSurfaceFormatKHR& format : formats)
            {
                if (format.format == kRequiredSwapchainFormat)
                {
                    chosenFormat = format;
                    return true;
                }
            }

            return false;
        }

        bool SupportsOptimalFormatFeatures(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatFeatureFlags requiredFeatures)
        {
            VkFormatProperties properties;
            vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &properties);
            return (properties.optimalTilingFeatures & requiredFeatures) == requiredFeatures;
        }

        bool SupportsRequiredSwapchainSurface(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* outCaps = nullptr, VkSurfaceFormatKHR* outFormat = nullptr)
        {
            VkSurfaceCapabilitiesKHR caps;
            VKR(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &caps));

            if ((caps.supportedUsageFlags & kRequiredSwapchainUsage) != kRequiredSwapchainUsage)
            {
                return false;
            }

            VkSurfaceFormatKHR chosenFormat;
            if (TryChooseRequiredSwapchainFormat(physicalDevice, surface, chosenFormat) == false)
            {
                return false;
            }

            if (SupportsOptimalFormatFeatures(physicalDevice, chosenFormat.format, VK_FORMAT_FEATURE_BLIT_DST_BIT) == false)
            {
                return false;
            }

            if (SupportsOptimalFormatFeatures(physicalDevice, kRequiredRaytracingOutputFormat, VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT) == false)
            {
                return false;
            }

            if (outCaps != nullptr)
            {
                *outCaps = caps;
            }

            if (outFormat != nullptr)
            {
                *outFormat = chosenFormat;
            }

            return true;
        }
    }

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

        auto extensionNames = app.GetVulkanExtensionNames() | Linq::Concat(kExtensions);
        std::vector<const char*> extensions;
        for (const char* extensionName : extensionNames)
        {
            extensions.emplace_back(extensionName);
        }
        
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

    }

    void VkGraphics::InitializeDevice(VkSurfaceKHR surface)
    {
        if (m_Device.Get() != VK_NULL_HANDLE)
        {
            return;
        }

        uint32_t gpuCount = 0;
        VKR(vkEnumeratePhysicalDevices(m_Instance, &gpuCount, nullptr));
        if (gpuCount == 0)
        {
            throw InvalidOperationException(TEXT("No Vulkan physical devices were found."));
        }

        std::vector<VkPhysicalDevice> physicalDevices{ gpuCount };
        VKR(vkEnumeratePhysicalDevices(m_Instance, &gpuCount, physicalDevices.data()));

        VkPhysicalDevice selectedPhysicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties selectedPhysicalDeviceProps = {};
        std::array<QueueFamilyInfo, 3> selectedQueueFamilyInfos =
        {
            MakeInvalidQueueFamilyInfo(),
            MakeInvalidQueueFamilyInfo(),
            MakeInvalidQueueFamilyInfo()
        };
        int32 selectedDeviceScore = -1;

        for (size_t i = 0; i < physicalDevices.size(); ++i)
        {
            auto physicalDevice = physicalDevices[i];
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(physicalDevice, &props);
            LogVulkan::Verbose(TEXT("Physical Device #{}: {} ({})"), i, String::FromCodepage(props.deviceName), FormatDeviceType(props.deviceType));

            uint32_t extensionsCount = 0;
            VKR(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, nullptr));

            std::vector<VkExtensionProperties> pdExtensions{ (size_t)extensionsCount };
            VKR(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, pdExtensions.data()));

            LogVulkan::Verbose(TEXT("  Extensions: "));
            for (auto& extension : pdExtensions)
            {
                LogVulkan::Verbose(TEXT("    {}"), String::FromCodepage(extension.extensionName));
            }

            if (SupportsRequiredDeviceExtensions(physicalDevice) == false)
            {
                LogVulkan::Verbose(TEXT("  Skipped: missing required Vulkan raytracing device extension."));
                continue;
            }

            if (SupportsRequiredRaytracingFeatures(physicalDevice) == false)
            {
                LogVulkan::Verbose(TEXT("  Skipped: missing required Vulkan raytracing feature."));
                continue;
            }

            std::array<QueueFamilyInfo, 3> queueFamilyInfos;
            if (TryFindQueueFamilies(physicalDevice, surface, queueFamilyInfos) == false)
            {
                LogVulkan::Verbose(TEXT("  Skipped: missing required graphics, compute, transfer, or present queue support."));
                continue;
            }

            if (SupportsRequiredSwapchainSurface(physicalDevice, surface) == false)
            {
                LogVulkan::Verbose(TEXT("  Skipped: missing required swapchain format, transfer destination, or raytracing output support."));
                continue;
            }

            int32 score = ScorePhysicalDevice(props);
            if (selectedPhysicalDevice == VK_NULL_HANDLE || score > selectedDeviceScore)
            {
                selectedPhysicalDevice = physicalDevice;
                selectedPhysicalDeviceProps = props;
                selectedQueueFamilyInfos = queueFamilyInfos;
                selectedDeviceScore = score;
            }
        }

        if (selectedPhysicalDevice == VK_NULL_HANDLE)
        {
            throw InvalidOperationException(TEXT("No Vulkan physical device supports required raytracing, presentation, and swapchain transfer capabilities."));
        }

        ValidateRequiredDeviceExtensions(selectedPhysicalDevice, selectedPhysicalDeviceProps.deviceName);
        ValidateRequiredRaytracingFeatures(selectedPhysicalDevice, selectedPhysicalDeviceProps.deviceName);

        m_RaytracingPipelineProperties =
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR,
            .pNext = &m_AccelerationStructureProperties
        };
        m_AccelerationStructureProperties =
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR
        };

        VkPhysicalDeviceProperties2 selectedDeviceProperties =
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
            .pNext = &m_RaytracingPipelineProperties
        };
        vkGetPhysicalDeviceProperties2(selectedPhysicalDevice, &selectedDeviceProperties);
        m_RaytracingPipelineProperties.pNext = nullptr;

        std::array<QueueFamilyInfo, 3> queueFamilyInfos = selectedQueueFamilyInfos;
        QueueFamilyInfo& bestPrimaryQueueFamily = queueFamilyInfos[0];
        QueueFamilyInfo& bestComputeQueueFamily = queueFamilyInfos[1];
        QueueFamilyInfo& bestTransferQueueFamily = queueFamilyInfos[2];

        LogVulkan::Verbose(TEXT("Graphics queue family index: {} (supports {})"), bestPrimaryQueueFamily.FamilyIndex, bestPrimaryQueueFamily.SupportsCount);
        LogVulkan::Verbose(TEXT("Compute queue family index: {} (supports {})"), bestComputeQueueFamily.FamilyIndex, bestComputeQueueFamily.SupportsCount);
        LogVulkan::Verbose(TEXT("Transfer queue family index: {} (supports {})"), bestTransferQueueFamily.FamilyIndex, bestTransferQueueFamily.SupportsCount);

        struct QueueCreateInfo
        {
            uint32_t FamilyIndex;
            std::vector<int32> Types;
            std::vector<float> Priorities;
        };

        std::vector<QueueCreateInfo> queueCreateInfos;
        queueCreateInfos.reserve(3);

        for (size_t i = 0; i < 3; ++i)
        {
			auto& familyInfo = queueFamilyInfos[i];
            QueueCreateInfo* createInfo = nullptr;
            for (auto& existingInfo : queueCreateInfos)
            {
                if (existingInfo.FamilyIndex == familyInfo.FamilyIndex)
                {
                    createInfo = &existingInfo;
                    break;
                }
			}

            if (createInfo == nullptr)
            {
                createInfo = &queueCreateInfos.emplace_back();
                createInfo->FamilyIndex = familyInfo.FamilyIndex;
            }

            createInfo->Types.emplace_back((int32)i);
            createInfo->Priorities.emplace_back(1.0f);
        }

        std::vector<VkDeviceQueueCreateInfo> vkQueueInfos;
		for (auto& createInfo : queueCreateInfos)
        {
            vkQueueInfos.emplace_back(VkDeviceQueueCreateInfo
            {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = createInfo.FamilyIndex,
                .queueCount = (uint32_t)createInfo.Types.size(),
                .pQueuePriorities = createInfo.Priorities.data()
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

        VkDeviceCreateInfo vkDeviceInfo =
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = &timelineFeatures,
             .queueCreateInfoCount = (uint32_t)vkQueueInfos.size(),
             .pQueueCreateInfos = vkQueueInfos.data(),
             .enabledExtensionCount = (uint32_t)kRequiredDeviceExtensions.size(),
             .ppEnabledExtensionNames = kRequiredDeviceExtensions.data()
         };

        LogVulkan::Verbose(TEXT("Logical device created using {} physical device."), String::FromCodepage(selectedPhysicalDeviceProps.deviceName));
        VKR(vkCreateDevice(selectedPhysicalDevice, &vkDeviceInfo, nullptr, &m_Device));

        m_vkCreateRayTracingPipelinesKHR = LoadRequiredDeviceFunction<PFN_vkCreateRayTracingPipelinesKHR>(m_Device, "vkCreateRayTracingPipelinesKHR");
        m_vkGetRayTracingShaderGroupHandlesKHR = LoadRequiredDeviceFunction<PFN_vkGetRayTracingShaderGroupHandlesKHR>(m_Device, "vkGetRayTracingShaderGroupHandlesKHR");
        m_vkCmdTraceRaysKHR = LoadRequiredDeviceFunction<PFN_vkCmdTraceRaysKHR>(m_Device, "vkCmdTraceRaysKHR");
        m_vkGetBufferDeviceAddress = LoadRequiredDeviceFunction<PFN_vkGetBufferDeviceAddress>(m_Device, "vkGetBufferDeviceAddress");
        m_vkCreateAccelerationStructureKHR = LoadRequiredDeviceFunction<PFN_vkCreateAccelerationStructureKHR>(m_Device, "vkCreateAccelerationStructureKHR");
        m_vkDestroyAccelerationStructureKHR = LoadRequiredDeviceFunction<PFN_vkDestroyAccelerationStructureKHR>(m_Device, "vkDestroyAccelerationStructureKHR");
        m_vkGetAccelerationStructureBuildSizesKHR = LoadRequiredDeviceFunction<PFN_vkGetAccelerationStructureBuildSizesKHR>(m_Device, "vkGetAccelerationStructureBuildSizesKHR");
        m_vkBuildAccelerationStructuresKHR = LoadRequiredDeviceFunction<PFN_vkBuildAccelerationStructuresKHR>(m_Device, "vkBuildAccelerationStructuresKHR");
        m_vkCmdBuildAccelerationStructuresKHR = LoadRequiredDeviceFunction<PFN_vkCmdBuildAccelerationStructuresKHR>(m_Device, "vkCmdBuildAccelerationStructuresKHR");
        m_vkGetAccelerationStructureDeviceAddressKHR = LoadRequiredDeviceFunction<PFN_vkGetAccelerationStructureDeviceAddressKHR>(m_Device, "vkGetAccelerationStructureDeviceAddressKHR");

        for (auto& createInfo : queueCreateInfos)
        {
            for (size_t i = 0; i < createInfo.Types.size(); ++i)
            {
                int32 type = createInfo.Types[i];
                check(m_Queues[(size_t)type] == nullptr);
                VkQueue queue;
                vkGetDeviceQueue(m_Device, createInfo.FamilyIndex, (uint32_t)i, &queue);
                auto& queueFamilyInfo = queueFamilyInfos[(size_t)type];
				m_Queues[(size_t)type] = std::make_unique<VkCommandQueue>(this, queue, queueFamilyInfo.FamilyIndex, queueFamilyInfo.Flags);
			}
		}

        m_PhysicalDevice = selectedPhysicalDevice;
    }

    VkGraphics::~VkGraphics() noexcept
    {
        Dispose();
    }

    void VkGraphics::Dispose() noexcept
    {
        m_Queues = {};
        m_PhysicalDevice = nullptr;
        m_RaytracingPipelineProperties = {};
        m_AccelerationStructureProperties = {};
        m_vkCreateRayTracingPipelinesKHR = nullptr;
        m_vkGetRayTracingShaderGroupHandlesKHR = nullptr;
        m_vkCmdTraceRaysKHR = nullptr;
        m_vkGetBufferDeviceAddress = nullptr;
        m_vkCreateAccelerationStructureKHR = nullptr;
        m_vkDestroyAccelerationStructureKHR = nullptr;
        m_vkGetAccelerationStructureBuildSizesKHR = nullptr;
        m_vkBuildAccelerationStructuresKHR = nullptr;
        m_vkCmdBuildAccelerationStructuresKHR = nullptr;
        m_vkGetAccelerationStructureDeviceAddressKHR = nullptr;
    }

    SharedPtr<GenericWindowSwapchainExtension> VkGraphics::InstallSwapChain(SharedPtr<GenericWindow> targetWindow)
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

        InitializeDevice(surface);
        
        VkSurfaceCapabilitiesKHR caps;
        VkSurfaceFormatKHR chosenFormat;
        if (SupportsRequiredSwapchainSurface(m_PhysicalDevice, surface, &caps, &chosenFormat) == false)
        {
            throw InvalidOperationException(TEXT("The Vulkan surface does not support the required swapchain format or transfer usage."));
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
            .imageFormat = chosenFormat.format,
            .imageColorSpace = chosenFormat.colorSpace,
            .imageExtent = caps.currentExtent,
            .imageArrayLayers = 1,
            .imageUsage = kRequiredSwapchainUsage,
            .preTransform = caps.currentTransform,
            .compositeAlpha = compositeAlpha,
            .presentMode = VK_PRESENT_MODE_FIFO_KHR,
            .clipped = VK_TRUE
        };

        VkQueue suitableQueue = VK_NULL_HANDLE;
        for (auto& queue : m_Queues)
        {
            VkBool32 supported;
            VKR(vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, queue->GetQueueFamilyIndex(), surface, &supported));
            if (supported)
            {
                suitableQueue = queue->GetVkQueue();
                break;
            }
        }

        if (suitableQueue == VK_NULL_HANDLE)
        {
			throw InvalidOperationException(TEXT("No suitable queue found for swapchain."));
        }

        VkSwapchainKHR swapchain;
        VKR(vkCreateSwapchainKHR(m_Device, &swapchainCreateInfo, nullptr, &swapchain));

        auto extension = New<VkSwapchainExt>(this, surface, swapchain, swapchainCreateInfo, suitableQueue);
        targetWindow->AddExtension(extension);
        return extension;
    }

    SharedPtr<GraphicsRenderPipeline> VkGraphics::CreateGeometryRenderPipeline(SharedPtr<Shader> shader)
    {
        return New<VkGeometryRenderPipeline>(this, std::move(shader));
    }

    SharedPtr<RaytracingRenderPipeline> VkGraphics::CreateRaytracingRenderPipeline(SharedPtr<Shader> shader)
    {
        return New<VkRaytracingRenderPipeline>(this, std::move(shader));
    }

    SharedPtr<Shader> VkGraphics::CreateShader(ShaderCreationInfo shaderCreationInfo)
    {
        return New<VkShader>(std::move(shaderCreationInfo));
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

    uint32_t VkGraphics::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
    {
        VkPhysicalDeviceMemoryProperties memProps;
        vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProps);

        for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
        {
            if ((typeFilter & (1u << i)) && (memProps.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        // Fallback: find any matching memory type
        for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
        {
            if (typeFilter & (1u << i))
            {
                return i;
            }
        }

        return UINT32_MAX;
    }
}
