// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkCommon.h"

namespace Ayla
{
	class VkGraphics;

	class VkCommandQueue
	{
	private:
		const VkGraphics* m_Owner;
		const VkQueue m_Queue;
		const uint32_t m_QueueFamilyIndex;
		const VkQueueFlags m_Flags;
		const int32 m_SupportsCount;

	public:
		VkCommandQueue(VkGraphics* owner, VkQueue vkQueue, uint32_t queueFamilyIndex, VkQueueFlags flags);
		~VkCommandQueue() noexcept;

		int32 SupportsCount() const noexcept { return m_SupportsCount; }
		VkQueue GetVkQueue() const noexcept { return m_Queue; }
		uint32_t GetQueueFamilyIndex() const noexcept { return m_QueueFamilyIndex; }
		bool IsSupportGraphics() const noexcept { return (m_Flags & VK_QUEUE_GRAPHICS_BIT) != 0; }
		bool IsSupportCompute() const noexcept { return (m_Flags & VK_QUEUE_COMPUTE_BIT) != 0; }
		bool IsSupportTransfer() const noexcept { return (m_Flags & VK_QUEUE_TRANSFER_BIT) != 0; }
	};
}