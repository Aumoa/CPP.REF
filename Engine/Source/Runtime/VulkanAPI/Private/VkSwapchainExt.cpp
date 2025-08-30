// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkSwapchainExt.h"
#include "VkGraphics.h"

namespace Ayla
{
    VkSwapchainExt::VkSwapchainExt(VkGraphics* owner, VkSurfaceKHR surface, VkSwapchainKHR swapchain, const VkSwapchainCreateInfoKHR& swapchainCreateInfo, VkQueue suitableQueue)
        : m_Owner(owner)
        , m_Surface(surface)
        , m_Swapchain(swapchain)
        , m_SwapchainCreateInfoCache(swapchainCreateInfo)
        , m_SuitableQueue(suitableQueue)
    {
    }

    VkSwapchainExt::~VkSwapchainExt() noexcept
    {
        checkf(m_Surface == nullptr, TEXT("Swapchain does not destroyed."));
    }

    void VkSwapchainExt::Present()
    {
        uint32_t imageIndex;
        VKR(vkAcquireNextImageKHR(m_Owner->GetDevice(), m_Swapchain, UINT64_MAX, m_Owner->GetSemaphore(), VK_NULL_HANDLE, &imageIndex));

        auto semaphore = m_Owner->GetSemaphore();
        VkPresentInfoKHR presentInfo
        {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &semaphore,
            .swapchainCount = 1,
            .pSwapchains = &m_Swapchain,
            .pImageIndices = &imageIndex
        };

        VKR(vkQueuePresentKHR(m_SuitableQueue, &presentInfo), VK_ERROR_SURFACE_LOST_KHR, VK_ERROR_OUT_OF_DATE_KHR);
    }

    void VkSwapchainExt::Destroy()
    {
        CleanupSwapChain();

        vkDestroySurfaceKHR(m_Owner->GetInstance(), m_Surface, nullptr);
        m_Surface = nullptr;
    }

    void VkSwapchainExt::OnResize(const Vector2N& newSize)
    {
        VkExtent2D newExtent{ .width = (uint32_t)newSize.X, .height = (uint32_t)newSize.Y };
        if (memcmp(&newExtent, &m_SwapchainCreateInfoCache.imageExtent, sizeof(VkExtent2D)) == 0)
        {
            return;
        }

        CleanupSwapChain();

        m_SwapchainCreateInfoCache.imageExtent = newExtent;
        VKR(vkCreateSwapchainKHR(m_Owner->GetDevice(), &m_SwapchainCreateInfoCache, nullptr, &m_Swapchain));
        PlatformProcess::OutputDebugString(String::Format(TEXT("Swapchain resized to {}"), newSize));
    }

    void VkSwapchainExt::CleanupSwapChain()
    {
        vkDestroySwapchainKHR(m_Owner->GetDevice(), m_Swapchain, nullptr);
        m_Swapchain = nullptr;
    }
}