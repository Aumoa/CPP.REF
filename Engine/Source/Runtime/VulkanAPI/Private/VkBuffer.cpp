// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkBuffer.h"
#include "VkGraphics.h"

namespace Ayla
{
	VkBuffer::VkBuffer(VkGraphics* graphics, BufferUsage usage)
		: m_Usage(usage)
	{
	}

	VkBuffer::~VkBuffer() noexcept
	{
	}

	SharedPtr<Buffer> VkGraphics::CreateBuffer_Implementation(BufferUsage usage)
	{
		return New<VkBuffer>(this, usage);
	}
}