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
        VkDevice m_Device{ nullptr };
        VkQueue m_GraphicsQueue{ nullptr };
        VkSemaphore m_Semaphore{ nullptr };

    public:
        VkGraphics(GenericApplication* app);
        virtual ~VkGraphics() noexcept override;

        virtual void InstallSwapChain(std::shared_ptr<GenericWindow> targetWindow) override;
    };
}