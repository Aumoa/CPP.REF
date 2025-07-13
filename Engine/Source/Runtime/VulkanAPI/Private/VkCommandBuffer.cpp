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
			.queueFamilyIndex = (uint32_t)graphics->GetQueueFamilyIndex()
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

	void VkCommandBuffer::BeginCommands()
	{
		VkCommandBufferBeginInfo beginInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
		};

		VKR(vkBeginCommandBuffer(m_CommandBuffer, &beginInfo));
	}

	void VkCommandBuffer::EndCommands()
	{
		VKR(vkEndCommandBuffer(m_CommandBuffer));

		VkSubmitInfo submitInfo
		{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.commandBufferCount = 1,
			.pCommandBuffers = &m_CommandBuffer
		};

		VKR(vkQueueSubmit(m_Graphics->GetGraphicsQueue(), 1, &submitInfo, m_Graphics->GetFence()));
	}
}