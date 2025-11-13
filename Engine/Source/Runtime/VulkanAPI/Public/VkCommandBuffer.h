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
		::VkCommandBuffer m_CommandBuffer{ nullptr };
		std::vector<VkSemaphore> m_RenderCompletedSemaphores;
		bool m_HasBegun{ false };

	public:
		VkSemaphore m_PresentCompletedSemaphore{ nullptr };

	public:
		VkCommandBuffer(VkGraphics* graphics);
		virtual ~VkCommandBuffer() noexcept override;

		virtual void BeginCommands_Implementation() override;
		virtual void EndCommands_Implementation() override;

		::VkCommandBuffer GetVkCommandBuffer() const noexcept { return m_CommandBuffer; }
		VkSemaphore GetRenderCompletedSemaphore(size_t frameIndex) const noexcept { return m_RenderCompletedSemaphores[frameIndex]; }
	};
}