// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12ConstantBuffer.h"

#if PLATFORM_WINDOWS

ND3D12ConstantBuffer::ND3D12ConstantBuffer(ID3D12Device& InDevice)
	: pDevice(&InDevice)
{
}

void ND3D12ConstantBuffer::Reserve(size_t InBufferSize)
{
	if (InBufferSize > BufferSize)
	{
		D3D12_RESOURCE_DESC BufferDesc =
		{
			.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
			.Width = (UINT64)InBufferSize,
			.Height = 1,
			.DepthOrArraySize = 1,
			.MipLevels = 1,
			.SampleDesc = { 1, 0 },
			.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR
		};

		ComPtr<ID3D12Resource> pNewBuffer;
		HR(pDevice->CreateCommittedResource(&UploadHeap, D3D12_HEAP_FLAG_NONE, &BufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&pNewBuffer)));

		void* pNewMapped;
		HR(pNewBuffer->Map(0, 0, &pNewMapped));

		if (pMapped)
		{
			memcpy(pNewMapped, pMapped, BufferSize);
		}

		std::swap(pBuffer, pNewBuffer);
		std::swap(pMapped, pNewMapped);

		BufferSize = InBufferSize;
	}
}

#endif