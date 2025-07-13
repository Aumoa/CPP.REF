// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graphics.h"
#include "VkCommon.h"

namespace Ayla
{
    class GenericApplication;

    class VULKANAPI_API VkGraphics : public Graphics
    {
    private:
        VkInstanceRef m_Instance;
        VkPhysicalDevice m_PhysicalDevice{ nullptr };
        VkDeviceRef m_Device;
        VkQueue m_GraphicsQueue{ nullptr };
		size_t m_QueueFamilyIndex{ (size_t)-1 };
        VkFence m_Fence{ nullptr };
        VkSemaphore m_Semaphore{ nullptr };

    public:
        VkGraphics();
        virtual ~VkGraphics() noexcept override;

        virtual std::shared_ptr<GenericWindowSwapchainExtension> InstallSwapChain(std::shared_ptr<GenericWindow> targetWindow) override;
        virtual void BeginRenderThread() override;
        virtual void EndRenderThread() override;

        VkInstance GetInstance() const noexcept { return m_Instance; }
        VkDevice GetDevice() const noexcept { return m_Device; }
        VkQueue GetGraphicsQueue() const noexcept { return m_GraphicsQueue; }
        size_t GetQueueFamilyIndex() const noexcept { return m_QueueFamilyIndex; }
        VkFence GetFence() const noexcept { return m_Fence; }
        VkSemaphore GetSemaphore() const noexcept { return m_Semaphore; }
    };
}