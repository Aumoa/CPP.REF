// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommandBuffer.h"
#include "VkCommon.h"
#include "VkCommandBuffer.gen.h"

namespace Ayla
{
	class VkGraphics;

	ACLASS()
	class VkCommandBuffer : public CommandBuffer
	{
		GENERATED_BODY()

	private:
		VkGraphics* m_Graphics{ nullptr };
		VkCommandPool m_CommandPool{ nullptr };
		std::vector<::VkCommandBuffer> m_CommandBuffers;
		std::vector<VkSemaphore> m_SignalSemaphores;
		std::vector<VkSemaphore> m_WaitSemaphores;
		bool m_HasBegun{ false };
		VkFence m_Fence = VK_NULL_HANDLE;

	public:
		VkCommandBuffer(VkGraphics* graphics, bool fence);
		virtual ~VkCommandBuffer() noexcept override;

		virtual void Dispose() noexcept override;

		virtual void BeginCommands_Implementation() override;
		virtual void EndCommands_Implementation() override;
		virtual void WaitForCompletion(const TimeSpan& timeout) override;

		void AddSignalSemaphore(VkSemaphore semaphore);
		void AddWaitSemaphore(VkSemaphore semaphore);

		::VkCommandBuffer GetVkCommandBuffer() const noexcept;
		VkFence GetFence() const noexcept { return m_Fence; }
	};
}