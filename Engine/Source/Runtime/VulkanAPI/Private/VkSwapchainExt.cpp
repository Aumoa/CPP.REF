// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkSwapchainExt.h"
#include "VkGraphics.h"
#include "VkCommandBuffer.h"
#include "VkSwapchainRenderTexture.h"
#include <limits>

namespace Ayla
{
    namespace
    {
        bool IsZeroExtent(VkExtent2D extent) noexcept
        {
            return extent.width == 0 || extent.height == 0;
        }

        uint32_t ClampExtentValue(int32 desired, uint32_t minValue, uint32_t maxValue) noexcept
        {
            uint32_t value = static_cast<uint32_t>(desired);
            if (value < minValue)
            {
                return minValue;
            }

            if (maxValue != 0 && value > maxValue)
            {
                return maxValue;
            }

            return value;
        }

        VkExtent2D ResolveSwapchainExtent(const VkSurfaceCapabilitiesKHR& caps, const Vector2N& desiredSize) noexcept
        {
            if (caps.currentExtent.width != std::numeric_limits<uint32_t>::max())
            {
                return caps.currentExtent;
            }

            if (desiredSize.X <= 0 || desiredSize.Y <= 0)
            {
                return {};
            }

            return VkExtent2D
            {
                .width = ClampExtentValue(desiredSize.X, caps.minImageExtent.width, caps.maxImageExtent.width),
                .height = ClampExtentValue(desiredSize.Y, caps.minImageExtent.height, caps.maxImageExtent.height)
            };
        }
    }

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
        if (m_PendingFrameNumber == -1 || m_PendingFrameNumber > (int64)m_Owner->GetFrameNumber() || (!m_PendingResize.has_value() && !m_RecreateRequested))
        {
            return;
        }

        vkDeviceWaitIdle(m_Owner->GetDevice());
        m_PendingFrameNumber = -1;
        bool recreateRequested = m_RecreateRequested;
        m_RecreateRequested = false;

        auto newSize = m_PendingResize.value_or(GetSize());
        m_PendingResize.reset();

        VkSurfaceCapabilitiesKHR caps;
        VKR(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Owner->GetPhysicalDevice(), m_Surface, &caps));

        VkExtent2D newExtent = ResolveSwapchainExtent(caps, newSize);
        if (IsZeroExtent(newExtent))
        {
            m_IsPresentable = false;
            LogVulkan::Verbose(TEXT("Skipped Vulkan swapchain resize while surface extent is zero."));
            return;
        }

        if (memcmp(&newExtent, &m_SwapchainCreateInfoCache.imageExtent, sizeof(VkExtent2D)) == 0 && m_IsPresentable && !recreateRequested)
        {
            return;
        }

        VkSwapchainCreateInfoKHR newCreateInfo = m_SwapchainCreateInfoCache;
        newCreateInfo.imageExtent = newExtent;
        newCreateInfo.preTransform = caps.currentTransform;
        newCreateInfo.oldSwapchain = m_Swapchain;

        VkSwapchainKHR newSwapchain = VK_NULL_HANDLE;
        VkResult result = vkCreateSwapchainKHR(m_Owner->GetDevice(), &newCreateInfo, nullptr, &newSwapchain);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            m_IsPresentable = false;
            RequestRecreate();
            return;
        }
        VKR(result);

        m_SwapchainRenderTexture->Invalidate();
        CleanupSwapchain();

        newCreateInfo.oldSwapchain = VK_NULL_HANDLE;
        m_SwapchainCreateInfoCache = newCreateInfo;
        m_Swapchain = newSwapchain;
        m_IsPresentable = true;
        LogVulkan::Verbose(TEXT("Swapchain resized to {}"), newSize);
    }

    void VkSwapchainExt::OnResize(const Vector2N& newSize)
    {
        m_PendingResize = newSize;
        m_PendingFrameNumber = m_Owner->GetFrameNumber() + 1;
    }

    void VkSwapchainExt::RequestRecreate()
    {
        m_RecreateRequested = true;
        if (m_PendingFrameNumber == -1)
        {
            m_PendingFrameNumber = m_Owner->GetFrameNumber() + 1;
        }
    }

    Vector2N VkSwapchainExt::GetSize() const
    {
        auto extent = m_SwapchainCreateInfoCache.imageExtent;
        return Vector2N((int32)extent.width, (int32)extent.height);
    }

    void VkSwapchainExt::CleanupSwapchain()
    {
        if (m_Swapchain != VK_NULL_HANDLE)
        {
            vkDestroySwapchainKHR(m_Owner->GetDevice(), m_Swapchain, nullptr);
        }
        m_Swapchain = nullptr;
    }
}
