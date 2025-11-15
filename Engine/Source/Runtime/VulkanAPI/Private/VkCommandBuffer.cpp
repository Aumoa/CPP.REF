// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkCommandBuffer.h"
#include "VkGraphics.h"

namespace Ayla
{
	VkCommandBuffer::VkCommandBuffer(VkGraphics* graphics)
		: m_Graphics{ graphics }
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = (uint32_t)graphics->GetGraphicsQueueFamilyIndex()
		};

		VKR(vkCreateCommandPool(graphics->GetDevice(), &commandPoolCreateInfo, nullptr, &m_CommandPool));

		VkCommandBufferAllocateInfo commandBufferAllocInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = m_CommandPool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		};

		VKR(vkAllocateCommandBuffers(graphics->GetDevice(), &commandBufferAllocInfo, &m_CommandBuffer));

		VkSemaphoreCreateInfo semaphoreCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
		};
		m_RenderCompletedSemaphores.resize(VkGraphics::kMaxSwapchainImages);
		for (auto& semaphore : m_RenderCompletedSemaphores)
		{
			VKR(vkCreateSemaphore(graphics->GetDevice(), &semaphoreCreateInfo, nullptr, &semaphore));
		}
	}

	VkCommandBuffer::~VkCommandBuffer() noexcept
	{
		if (m_CommandBuffer)
		{
			vkFreeCommandBuffers(m_Graphics->GetDevice(), m_CommandPool, 1, &m_CommandBuffer);
			m_CommandBuffer = nullptr;
		}

		if (m_CommandPool)
		{
			vkDestroyCommandPool(m_Graphics->GetDevice(), m_CommandPool, nullptr);
			m_CommandPool = nullptr;
		}
	}

	void VkCommandBuffer::BeginCommands_Implementation()
	{
		VkCommandBufferBeginInfo beginInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
		};

		vkResetCommandBuffer(m_CommandBuffer, 0);
		VKR(vkBeginCommandBuffer(m_CommandBuffer, &beginInfo));
	}

	void VkCommandBuffer::EndCommands_Implementation()
	{
		VKR(vkEndCommandBuffer(m_CommandBuffer));

		auto semaphore = GetRenderCompletedSemaphore();
		VkSubmitInfo submitInfo
		{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.commandBufferCount = 1,
			.pCommandBuffers = &m_CommandBuffer,
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = &semaphore
		};

		VkSemaphore presentCompletedSemaphore = m_PresentCompletedSemaphore;
		m_PresentCompletedSemaphore = nullptr;
		if (presentCompletedSemaphore != nullptr)
		{
			VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			submitInfo.pWaitDstStageMask = &waitStage;
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = &presentCompletedSemaphore;
		}

		VKR(vkQueueSubmit(m_Graphics->GetGraphicsQueue(), 1, &submitInfo, m_Graphics->GetFence()));
	}

	VkSemaphore VkCommandBuffer::GetRenderCompletedSemaphore() const noexcept
	{
		return m_RenderCompletedSemaphores[m_Graphics->GetFrameNumber() % VkGraphics::kMaxSwapchainImages];
	}
}