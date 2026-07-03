// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkSwapchainExt.h"
#include "VkGraphics.h"
#include "VkCommandBuffer.h"
#include "VkSwapchainRenderTexture.h"

namespace Ayla
{
    VkSwapchainExt::VkSwapchainExt(VkGraphics* owner, VkSurfaceKHR surface, VkSwapchainKHR swapchain, const VkSwapchainCreateInfoKHR& swapchainCreateInfo, VkQueue suitableQueue)
        : m_Owner(owner)
        , m_Surface(surface)
        , m_Swapchain(swapchain)
        , m_SwapchainCreateInfoCache(swapchainCreateInfo)
        , m_SuitableQueue(suitableQueue)
    {
        m_SwapchainRenderTexture = New<VkSwapchainRenderTexture>(this, owner);
    }

    VkSwapchainExt::~VkSwapchainExt() noexcept
    {
        checkf(m_Surface == nullptr, TEXT("Swapchain does not destroyed."));
    }

    PresentableRenderTarget* VkSwapchainExt::GetPresentableRenderTarget()
    {
        return m_SwapchainRenderTexture.Get();
    }

    void VkSwapchainExt::Destroy()
    {
        m_SwapchainRenderTexture->Dispose();
        CleanupSwapchain();
        vkDestroySurfaceKHR(m_Owner->GetInstance(), m_Surface, nullptr);
        m_Surface = nullptr;
    }

    void VkSwapchainExt::DoResize()
    {
        if (m_PendingFrameNumber == -1 || m_PendingFrameNumber > (int64)m_Owner->GetFrameNumber() || !m_PendingResize.has_value())
        {
            return;
        }

        vkDeviceWaitIdle(m_Owner->GetDevice());
        m_PendingFrameNumber = -1;

        auto newSize = m_PendingResize.value();
        m_PendingResize.reset();

        VkExtent2D newExtent{ .width = (uint32_t)newSize.X, .height = (uint32_t)newSize.Y };
        if (memcmp(&newExtent, &m_SwapchainCreateInfoCache.imageExtent, sizeof(VkExtent2D)) == 0)
        {
            return;
        }

        m_SwapchainRenderTexture->Invalidate();
        CleanupSwapchain();

        m_SwapchainCreateInfoCache.imageExtent = newExtent;
        VKR(vkCreateSwapchainKHR(m_Owner->GetDevice(), &m_SwapchainCreateInfoCache, nullptr, &m_Swapchain));
        LogVulkan::Verbose(TEXT("Swapchain resized to {}"), newSize);
    }

    void VkSwapchainExt::OnResize(const Vector2N& newSize)
    {
        m_PendingResize = newSize;
        m_PendingFrameNumber = m_Owner->GetFrameNumber() + 1;
    }

    Vector2N VkSwapchainExt::GetSize() const
    {
        auto extent = m_SwapchainCreateInfoCache.imageExtent;
        return Vector2N((int32)extent.width, (int32)extent.height);
    }

    void VkSwapchainExt::CleanupSwapchain()
    {
        vkDestroySwapchainKHR(m_Owner->GetDevice(), m_Swapchain, nullptr);
        m_Swapchain = nullptr;
    }
}
