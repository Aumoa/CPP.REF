// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graphics.h"
#include "VkCommon.h"
#include "VkGraphics.gen.h"

namespace Ayla
{
    class GenericApplication;

    ACLASS()
    class VULKANAPI_API VkGraphics : public Graphics
    {
        GENERATED_BODY()

    private:
        VkInstanceRef m_Instance;
        VkPhysicalDevice m_PhysicalDevice{ nullptr };
        VkDeviceRef m_Device;
        VkQueue m_GraphicsQueue{ nullptr };
		uint32_t m_GraphicsQueueFamilyIndex{ 0 };
        uint32_t m_QueueCount{ 0 };
        std::vector<VkFence> m_Fences;

        std::atomic<std::size_t> m_FrameCount = 0;

    public:
        ACONSTRUCTOR()
        VkGraphics();
        virtual ~VkGraphics() noexcept override;

        virtual void Dispose() noexcept override;
        virtual RenderFeatures GetCurrentRenderFeature() noexcept override { return RenderFeatures::Vulkan; }

        virtual SharedPtr<GenericWindowSwapchainExtension> InstallSwapChain_Implementation(SharedPtr<GenericWindow> targetWindow) override;
        virtual SharedPtr<CommandBuffer> CreateCommandBuffer_Implementation() override;  // VkCommandBuffer.cpp

        virtual void BeginRenderFrame() override;
        virtual void EndRenderFrame() override;
        virtual void WaitForCompletion() override;

        VkInstance GetInstance() const noexcept { return m_Instance; }
        VkDevice GetDevice() const noexcept { return m_Device; }
        VkQueue GetGraphicsQueue() const noexcept { return m_GraphicsQueue; }
		uint32_t GetGraphicsQueueFamilyIndex() const noexcept { return m_GraphicsQueueFamilyIndex; }
        VkFence GetFence() const noexcept;

        inline size_t GetFrameNumber() const noexcept { return m_FrameCount; }
        inline size_t GetFrameIndex() const noexcept { return m_FrameCount % kMaxFramesInFlight; }

        PFN_vkSetDebugUtilsObjectNameEXT GetSetDebugUtilsObjectNameEXTFunction() const noexcept;
    };
}