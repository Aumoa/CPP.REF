// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkCommon.h"
#include "GenericPlatform/GenericWindowSwapchainExtension.h"
#include "GenericPlatform/IGenericWindowResizeEventHandler.h"

namespace Ayla
{
    class VkGraphics;

    class VkSwapchainExt : public GenericWindowSwapchainExtension, public IGenericWindowResizeEventHandler
    {
    private:
        VkGraphics* m_Owner;
        VkSurfaceKHR m_Surface;
        VkSwapchainKHR m_Swapchain;
        VkSwapchainCreateInfoKHR m_SwapchainCreateInfoCache;

    public:
        VkSwapchainExt(VkGraphics* owner, VkSurfaceKHR surface, VkSwapchainKHR swapchain, const VkSwapchainCreateInfoKHR& swapchainCreateInfo);
        virtual ~VkSwapchainExt() noexcept override;

        virtual void Present() override;

        virtual void OnResize(const Vector2N& newSize) override;

    private:
        void CleanupSwapChain();
    };
}