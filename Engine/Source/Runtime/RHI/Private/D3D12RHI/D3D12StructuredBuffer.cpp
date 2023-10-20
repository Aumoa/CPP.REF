// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12StructuredBuffer.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Global.h"
#include "D3D12RHI/D3D12Graphics.h"

ND3D12StructuredBuffer::ND3D12StructuredBuffer(ID3D12Device& InDevice, size_t BufferSize)
	: BufferSize(BufferSize)
{
	auto Desc = GetBufferDesc(BufferSize);
	HR(InDevice.CreateCommittedResource(&DefaultHeap, D3D12_HEAP_FLAG_NONE, &Desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&pResource)));

	HR(InDevice.CreateCommittedResource(&UploadHeap, D3D12_HEAP_FLAG_NONE, &Desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&pUploadHeap)));
	HR(pUploadHeap->Map(0, nullptr, &pUploadPtr));
}

void ND3D12StructuredBuffer::UpdateRange(size_t Offset, std::span<const uint8> Buf)
{
	int8* p = (int8*)pUploadPtr + Offset;
	memcpy(p, Buf.data(), Buf.size_bytes());
}

Task<> ND3D12StructuredBuffer::CommitAsync()
{
	return ND3D12Global::GetDynamicRHI().EnqueueGraphicsCommandAsync([pResource = pResource, pUploadHeap = pUploadHeap](ID3D12GraphicsCommandList* pCmd)
	{
		D3D12_RESOURCE_BARRIER Barrier =
		{
			.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
			.Transition =
			{
				.pResource = pResource.Get(),
				.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
				.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST
			}
		};

		pCmd->ResourceBarrier(1, &Barrier);
		pCmd->CopyResource(pResource.Get(), pUploadHeap.Get());
		std::swap(Barrier.Transition.StateBefore, Barrier.Transition.StateAfter);
		pCmd->ResourceBarrier(1, &Barrier);

		return false;
	});
}

#endif