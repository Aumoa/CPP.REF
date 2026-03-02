// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12Buffer.h"

namespace Ayla
{
	D3D12Buffer::D3D12Buffer(ComPtr<ID3D12Resource> buffer, BufferUsage usage)
		: m_Buffer(buffer), m_Usage(usage)
	{
	}

	D3D12Buffer::~D3D12Buffer() noexcept
	{
	}
}