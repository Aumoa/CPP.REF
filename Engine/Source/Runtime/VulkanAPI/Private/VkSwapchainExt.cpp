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
        m_SwapchainRenderTexture = New<VkSwapchainRenderTexture>(this);
        ReallocateSwapchainImages();

        m_PresentCompletedSemaphores.resize(VkGraphics::kMaxFramesInFlight);
        for (auto& semaphore : m_PresentCompletedSemaphores)
        {
            VkSemaphoreCreateInfo semaphoreCreateInfo{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
            VKR(vkCreateSemaphore(m_Owner->GetDevice(), &semaphoreCreateInfo, nullptr, &semaphore));
        }
    }

    VkSwapchainExt::~VkSwapchainExt() noexcept
    {
        checkf(m_Surface == nullptr, TEXT("Swapchain does not destroyed."));
    }

    SharedPtr<RenderTexture> VkSwapchainExt::GetRenderTexture()
    {
        return m_SwapchainRenderTexture;
    }

    void VkSwapchainExt::Present(CommandBuffer* commandBuffer)
    {
        m_SwapchainRenderTexture->Present(m_SuitableQueue, (VkCommandBuffer*)commandBuffer);
    }

    void VkSwapchainExt::Destroy()
    {
        CleanupSwapchain();

        for (auto& semaphore : m_PresentCompletedSemaphores)
        {
            vkDestroySemaphore(m_Owner->GetDevice(), semaphore, nullptr);
		}

		m_PresentCompletedSemaphores.clear();
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

        CleanupSwapchain();

        m_SwapchainCreateInfoCache.imageExtent = newExtent;
        VKR(vkCreateSwapchainKHR(m_Owner->GetDevice(), &m_SwapchainCreateInfoCache, nullptr, &m_Swapchain));
        LogVulkan::Verbose(TEXT("Swapchain resized to {}"), newSize);

        ReallocateSwapchainImages();
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

    void VkSwapchainExt::ReallocateSwapchainImages()
    {
        uint32_t imageCount = 0;
        VKR(vkGetSwapchainImagesKHR(m_Owner->GetDevice(), m_Swapchain, &imageCount, nullptr));
        m_SwapchainImages.resize(imageCount);
        VKR(vkGetSwapchainImagesKHR(m_Owner->GetDevice(), m_Swapchain, &imageCount, m_SwapchainImages.data()));
	}
}