// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkSwapchainExt.h"
#include "VkGraphics.h"
#include "VkCommandBuffer.h"

namespace Ayla
{
    VkSwapchainExt::VkSwapchainExt(VkGraphics* owner, VkSurfaceKHR surface, VkSwapchainKHR swapchain, const VkSwapchainCreateInfoKHR& swapchainCreateInfo, VkQueue suitableQueue)
        : m_Owner(owner)
        , m_Surface(surface)
        , m_Swapchain(swapchain)
        , m_SwapchainCreateInfoCache(swapchainCreateInfo)
        , m_SuitableQueue(suitableQueue)
    {
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

    void VkSwapchainExt::Acquire(CommandBuffer* commandBuffer)
    {
        check(m_CurrentImageIndex == 0xFFFFFFFF);
        
		auto presentCompletedSemaphore = m_PresentCompletedSemaphores[m_Owner->GetFrameIndex()];
        VKR(vkAcquireNextImageKHR(m_Owner->GetDevice(), m_Swapchain, UINT64_MAX, presentCompletedSemaphore, VK_NULL_HANDLE, &m_CurrentImageIndex));

		bool isFirstRender = (m_SwapchainImageFirstRender & (1 << m_CurrentImageIndex)) == 0;
        auto* vkCmd = (VkCommandBuffer*)commandBuffer;

        if (isFirstRender)
        {
            VkImageMemoryBarrier barrier
            {
                .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dstAccessMask = 0,
                .oldLayout = isFirstRender ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .image = m_SwapchainImages[m_CurrentImageIndex],
                .subresourceRange =
                {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1,
                },
            };

            vkCmdPipelineBarrier(
                vkCmd->GetVkCommandBuffer(),
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                0,
                0, nullptr,
                0, nullptr,
                1, &barrier
            );
        }

		m_SwapchainImageFirstRender |= (1 << m_CurrentImageIndex);
        vkCmd->m_PresentCompletedSemaphore = presentCompletedSemaphore;
    }

    void VkSwapchainExt::Present(CommandBuffer* commandBuffer)
    {
        check(m_CurrentImageIndex != 0xFFFFFFFF);
        auto semaphore = ((VkCommandBuffer*)commandBuffer)->GetRenderCompletedSemaphore();
        VkPresentInfoKHR presentInfo
        {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &semaphore,
            .swapchainCount = 1,
            .pSwapchains = &m_Swapchain,
            .pImageIndices = &m_CurrentImageIndex
        };

        VKR(vkQueuePresentKHR(m_SuitableQueue, &presentInfo), VK_ERROR_SURFACE_LOST_KHR, VK_ERROR_OUT_OF_DATE_KHR);
        m_CurrentImageIndex = 0xFFFFFFFF;
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