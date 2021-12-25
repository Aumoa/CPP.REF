// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12Resource.h"
#include "RHI/IRHIBuffer.h"

class DIRECTX_API SD3D12Buffer : public SD3D12Resource, implements IRHIBuffer
{
	GENERATED_BODY(SD3D12Buffer)

private:
	ComPtr<ID3D12Resource> Resource;
	ComPtr<ID3D12Resource> UploadHeap;
	RHIBufferDesc Desc;
	uint64 TotalBytes = 0;

public:
	SD3D12Buffer(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12Resource> InResource, ComPtr<ID3D12Resource> InUploadHeap, const RHIBufferDesc& InDesc);

	virtual RHIBufferDesc GetDesc() override { return Desc; }

	virtual void UpdateSubresource(SD3D12CommandList* InCommandList, uint32 InSubresource, const RHISubresourceData* InUploadData) override;

	static void UpdateSubresource(
		SD3D12CommandList* InCommandList,
		ID3D12Resource* InBuf,
		ID3D12Resource* InUploadBuf,
		uint32 InSubresource,
		uint64 InTotalBytes,
		const RHISubresourceData* InUploadData);

protected:
	virtual void Dispose(bool bDisposing) override;
};