// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkCommandBuffer.h"
#include "VkGraphics.h"
#include "VkCommandQueue.h"

namespace Ayla
{
	VkCommandBuffer::VkCommandBuffer(VkGraphics* graphics, bool fence)
		: m_Graphics{ graphics }
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = (uint32_t)graphics->GetGraphicsQueue()->GetQueueFamilyIndex()
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
		VKR(vkAllocateCommandBuffers(graphics->GetDevice(), &commandBufferAllocInfo, m_CommandBuffers.data()));

		if (fence)
		{
			VkFenceCreateInfo fenceCreateInfo
			{
				.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
				.flags = VK_FENCE_CREATE_SIGNALED_BIT
			};

			m_Fences.resize(VkGraphics::kMaxFramesInFlight);
			for (size_t i = 0; i < VkGraphics::kMaxFramesInFlight; ++i)
			{
				VKR(vkCreateFence(graphics->GetDevice(), &fenceCreateInfo, nullptr, &m_Fences[i]));
			}
		}
	}

	VkCommandBuffer::~VkCommandBuffer() noexcept
	{
		Dispose();
	}

	void VkCommandBuffer::Dispose() noexcept
	{
		for (auto& fence : m_Fences)
		{
			vkDestroyFence(m_Graphics->GetDevice(), fence, nullptr);
		}
		m_Fences.clear();

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
	}

	void VkCommandBuffer::BeginCommands_Implementation()
	{
		auto frameIndex = m_Graphics->GetFrameIndex();

		if (m_Fences.size() > 0)
		{
			VKR(vkResetFences(m_Graphics->GetDevice(), 1, &m_Fences[frameIndex]));
		}

		m_SignalSemaphores.clear();
		m_WaitSemaphores.clear();

		VkCommandBufferBeginInfo beginInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
		};

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
			.pCommandBuffers = &commandBuffer
		};

		submitInfo.signalSemaphoreCount = (uint32_t)m_SignalSemaphores.size();
		submitInfo.pSignalSemaphores = m_SignalSemaphores.data();

		static thread_local std::vector<VkPipelineStageFlags> sStages;
		sStages.resize(m_WaitSemaphores.size(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		submitInfo.pWaitDstStageMask = sStages.data();
		submitInfo.waitSemaphoreCount = (uint32_t)m_WaitSemaphores.size();
		submitInfo.pWaitSemaphores = m_WaitSemaphores.data();
		auto fence = m_Fences.size() > 0 ? m_Fences[frameIndex] : VK_NULL_HANDLE;
		VKR(vkQueueSubmit(m_Graphics->GetGraphicsQueue()->GetVkQueue(), 1, &submitInfo, fence));
	}

	void VkCommandBuffer::WaitForCompletion(const TimeSpan& timeout)
	{
		if (m_Fences.empty())
		{
			throw InvalidOperationException(TEXT("This command buffer was not created with a fence."));
		}

		auto frameIndex = m_Graphics->GetFrameIndex();
		VKR(vkWaitForFences(m_Graphics->GetDevice(), 1, &m_Fences[frameIndex], VK_TRUE, (uint64_t)timeout.GetTotalNanoseconds()));
	}

	void VkCommandBuffer::AddSignalSemaphore(VkSemaphore semaphore)
	{
		m_SignalSemaphores.emplace_back(semaphore);
	}

	void VkCommandBuffer::AddWaitSemaphore(VkSemaphore semaphore)
	{
		m_WaitSemaphores.emplace_back(semaphore);
	}

	::VkCommandBuffer VkCommandBuffer::GetVkCommandBuffer() const noexcept
	{
		return m_CommandBuffers[m_Graphics->GetFrameIndex()];
	}

	VkFence VkCommandBuffer::GetFence() const noexcept
	{
		return m_Fences[m_Graphics->GetFrameIndex()];
	}

	SharedPtr<CommandBuffer> VkGraphics::CreateCommandBuffer_Implementation()
	{
		return New<VkCommandBuffer>(this, true);
	}
}