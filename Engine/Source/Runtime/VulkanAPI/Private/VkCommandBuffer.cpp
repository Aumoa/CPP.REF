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
			.commandBufferCount = (uint32_t)VkGraphics::kMaxFramesInFlight
		};

		VkSemaphoreCreateInfo semaphoreCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		};

		m_CommandBuffers.resize(VkGraphics::kMaxFramesInFlight);
		m_RenderCompletedSemaphores.resize(VkGraphics::kMaxFramesInFlight);
		VKR(vkAllocateCommandBuffers(graphics->GetDevice(), &commandBufferAllocInfo, m_CommandBuffers.data()));
		for (size_t i = 0; i < VkGraphics::kMaxFramesInFlight; i++)
		{
			VKR(vkCreateSemaphore(graphics->GetDevice(), &semaphoreCreateInfo, nullptr, &m_RenderCompletedSemaphores[i]));
		}
	}

	VkCommandBuffer::~VkCommandBuffer() noexcept
	{
		if (m_CommandBuffers.size() > 0)
		{
			vkFreeCommandBuffers(m_Graphics->GetDevice(), m_CommandPool, (uint32_t)m_CommandBuffers.size(), m_CommandBuffers.data());
			m_CommandBuffers.clear();
		}

		if (m_CommandPool)
		{
			vkDestroyCommandPool(m_Graphics->GetDevice(), m_CommandPool, nullptr);
			m_CommandPool = nullptr;
		}

		for (auto& semaphore : m_RenderCompletedSemaphores)
		{
			vkDestroySemaphore(m_Graphics->GetDevice(), semaphore, nullptr);
		}
		m_RenderCompletedSemaphores.clear();
	}

	void VkCommandBuffer::BeginCommands_Implementation()
	{
		VkCommandBufferBeginInfo beginInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
		};

		auto frameIndex = m_Graphics->GetFrameIndex();
		auto commandBuffer = m_CommandBuffers[frameIndex];

		vkResetCommandBuffer(commandBuffer, 0);
		VKR(vkBeginCommandBuffer(commandBuffer, &beginInfo));
	}

	void VkCommandBuffer::EndCommands_Implementation()
	{
		auto frameIndex = m_Graphics->GetFrameIndex();
		auto commandBuffer = m_CommandBuffers[frameIndex];
		VKR(vkEndCommandBuffer(commandBuffer));

		VkSubmitInfo submitInfo
		{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.commandBufferCount = 1,
			.pCommandBuffers = &commandBuffer,
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = &m_RenderCompletedSemaphores[frameIndex],
		};

		VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		submitInfo.pWaitDstStageMask = &waitStage;
		submitInfo.waitSemaphoreCount = (uint32_t)m_ImageReadySemaphores.size();
		submitInfo.pWaitSemaphores = m_ImageReadySemaphores.data();
		VKR(vkQueueSubmit(m_Graphics->GetGraphicsQueue(), 1, &submitInfo, m_Graphics->GetFence()));
		
		m_ImageReadySemaphores.clear();
	}

	void VkCommandBuffer::AddImageReadySemaphore(VkSemaphore semaphore)
	{
		m_ImageReadySemaphores.emplace_back(semaphore);
	}

	::VkCommandBuffer VkCommandBuffer::GetVkCommandBuffer() const noexcept
	{
		return m_CommandBuffers[m_Graphics->GetFrameIndex()];
	}

	VkSemaphore VkCommandBuffer::GetRenderCompletedSemaphore() const noexcept
	{
		return m_RenderCompletedSemaphores[m_Graphics->GetFrameIndex()];
	}

	SharedPtr<CommandBuffer> VkGraphics::CreateCommandBuffer_Implementation()
	{
		return New<VkCommandBuffer>(this);
	}
}