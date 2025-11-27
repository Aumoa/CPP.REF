// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Buffer.h"
#include "VkCommon.h"
#include "BufferUsage.h"
#include "VkBuffer.gen.h"

namespace Ayla
{
	class VkGraphics;

	ACLASS()
	class VkBuffer : public Buffer
	{
		GENERATED_BODY()

	private:
		const BufferUsage m_Usage;
		::VkBuffer m_Buffer = VK_NULL_HANDLE;
		VkDeviceMemory m_Memory = VK_NULL_HANDLE;

	public:
		VkBuffer(VkGraphics* graphics, BufferUsage usage);
		virtual ~VkBuffer() noexcept override;
	};
}