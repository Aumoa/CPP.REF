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

    public:
        static constexpr size_t kMaxFramesInFlight = 2;

    private:
        VkInstanceRef m_Instance;
        VkPhysicalDevice m_PhysicalDevice{ nullptr };
        VkDeviceRef m_Device;
        VkQueue m_GraphicsQueue{ nullptr };
		uint32_t m_GraphicsQueueFamilyIndex{ 0 };
        uint32_t m_QueueCount{ 0 };
        VkFence m_Fence{ nullptr };

        std::atomic<std::size_t> m_FrameCount = 0;
        std::vector<VkSemaphore> m_RenderCompletedSemaphores;

    public:
        ACONSTRUCTOR()
        VkGraphics();
        virtual ~VkGraphics() noexcept override;

        virtual SharedPtr<GenericWindowSwapchainExtension> InstallSwapChain_Implementation(SharedPtr<GenericWindow> targetWindow) override;
        virtual void BeginRenderFrame_Implementation() override;
        virtual void EndRenderFrame_Implementation() override;
        virtual SharedPtr<CommandBuffer> CreateCommandBuffer_Implementation() override;

        VkInstance GetInstance() const noexcept { return m_Instance; }
        VkDevice GetDevice() const noexcept { return m_Device; }
        VkQueue GetGraphicsQueue() const noexcept { return m_GraphicsQueue; }
		uint32_t GetGraphicsQueueFamilyIndex() const noexcept { return m_GraphicsQueueFamilyIndex; }
        VkFence GetFence() const noexcept { return m_Fence; }

        inline size_t GetFrameIndex() const noexcept { return m_FrameCount % kMaxFramesInFlight; }
        inline VkSemaphore GetRenderCompletedSemaphore(size_t index) const noexcept { return m_RenderCompletedSemaphores[index]; }
    };
}