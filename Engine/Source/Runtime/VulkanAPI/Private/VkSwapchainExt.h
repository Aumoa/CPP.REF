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
    class VkSwapchainRenderTexture;

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
        std::vector<VkImage> m_SwapchainImages;
        std::vector<VkSemaphore> m_PresentCompletedSemaphores;
        SharedPtr<VkSwapchainRenderTexture> m_SwapchainRenderTexture;

    public:
        VkSwapchainExt(VkGraphics* owner, VkSurfaceKHR surface, VkSwapchainKHR swapchain, const VkSwapchainCreateInfoKHR& swapchainCreateInfo, VkQueue suitableQueue);
        virtual ~VkSwapchainExt() noexcept override;

        virtual SharedPtr<RenderTexture> GetRenderTexture() override;
        virtual void Present(CommandBuffer* commandBuffer) override;
        virtual void Destroy() override;

        virtual void OnResize(const Vector2N& newSize) override;

        VkGraphics* GetOwner() const noexcept { return m_Owner; }
        VkSwapchainKHR GetSwapchain() const noexcept { return m_Swapchain; }
        Vector2N GetSize() const;

    private:
        void CleanupSwapchain();
		void ReallocateSwapchainImages();
    };
}