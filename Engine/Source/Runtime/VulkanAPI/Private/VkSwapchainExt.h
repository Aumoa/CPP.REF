// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkCommon.h"
#include "GenericWindowSwapchainExtension.h"
#include "IGenericWindowResizeEventHandler.h"
#include "VkSwapchainExt.gen.h"

namespace Ayla
{
    class VkGraphics;

    ACLASS()
    class VkSwapchainExt : public GenericWindowSwapchainExtension, public IGenericWindowResizeEventHandler
    {
        GENERATED_BODY()

    private:
        VkGraphics* m_Owner;
        VkSurfaceKHR m_Surface;
        VkSwapchainKHR m_Swapchain;
        VkSwapchainCreateInfoKHR m_SwapchainCreateInfoCache;
        VkQueue m_SuitableQueue;

    public:
        VkSwapchainExt(VkGraphics* owner, VkSurfaceKHR surface, VkSwapchainKHR swapchain, const VkSwapchainCreateInfoKHR& swapchainCreateInfo, VkQueue suitableQueue);
        virtual ~VkSwapchainExt() noexcept override;

        virtual void Present() override;
        virtual void Destroy() override;

        virtual void OnResize(const Vector2N& newSize) override;

    private:
        void CleanupSwapChain();
    };
}