// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkCommandQueue.h"

namespace Ayla
{
	VkCommandQueue::VkCommandQueue(VkGraphics* owner, VkQueue vkQueue, uint32_t queueFamilyIndex, VkQueueFlags flags)
		: m_Owner(owner)
		, m_Queue(vkQueue)
		, m_QueueFamilyIndex(queueFamilyIndex)
		, m_Flags(flags)
		, m_SupportsCount(
			(flags & VK_QUEUE_GRAPHICS_BIT ? 1 : 0) +
			(flags & VK_QUEUE_COMPUTE_BIT ? 1 : 0) +
			(flags & VK_QUEUE_TRANSFER_BIT ? 1 : 0))
	{
	}

	VkCommandQueue::~VkCommandQueue() noexcept
	{
	}
}