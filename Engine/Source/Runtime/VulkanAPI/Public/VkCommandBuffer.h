// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommandBuffer.h"
#include "VkCommon.h"

namespace Ayla
{
	class VkGraphics;

	class VkCommandBuffer : public CommandBuffer
	{
	private:
		VkGraphics* m_Graphics{ nullptr };
		VkCommandPool m_CommandPool{ nullptr };
		::VkCommandBuffer m_CommandBuffer{ nullptr };

	public:
		VkCommandBuffer(VkGraphics* graphics);
		virtual ~VkCommandBuffer() noexcept override;

		virtual void BeginCommands() override;
		virtual void EndCommands() override;
	};
}