// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12Buffer.h"
#include "BufferUsage.h"

namespace Ayla
{
	D3D12Buffer::D3D12Buffer(ComPtr<ID3D12Resource> buffer, BufferUsage usage, size_t byteSize, size_t stride)
		: m_Buffer(std::move(buffer))
		, m_Usage(usage)
		, m_ByteSize(byteSize)
		, m_Stride(stride)
	{
	}

	D3D12Buffer::~D3D12Buffer() noexcept
	{
	}

	void* D3D12Buffer::Map() const
	{
		void* p;
		HR(m_Buffer->Map(0, nullptr, &p));
		return p;
	}

	D3D12_VERTEX_BUFFER_VIEW D3D12Buffer::GetVertexBufferView() const noexcept
	{
		return D3D12_VERTEX_BUFFER_VIEW
		{
			.BufferLocation = m_Buffer->GetGPUVirtualAddress(),
			.SizeInBytes = static_cast<UINT>(m_ByteSize),
			.StrideInBytes = static_cast<UINT>(m_Stride)
		};
	}

	D3D12_INDEX_BUFFER_VIEW D3D12Buffer::GetIndexBufferView() const noexcept
	{
		return D3D12_INDEX_BUFFER_VIEW
		{
			.BufferLocation = m_Buffer->GetGPUVirtualAddress(),
			.SizeInBytes = static_cast<UINT>(m_ByteSize),
			.Format = (m_Stride == sizeof(uint32)) ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT
		};
	}
}