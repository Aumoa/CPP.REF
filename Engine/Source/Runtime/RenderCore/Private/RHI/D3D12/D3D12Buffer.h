// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12Resource.h"
#include "RHI/IRHIBuffer.h"

class SD3D12Buffer : public SD3D12Resource, implements IRHIBuffer
{
	GENERATED_BODY(SD3D12Buffer)

private:
	ComPtr<ID3D12Resource> _resource;
	ComPtr<ID3D12Resource> _uploadHeap;
	RHIBufferDesc _desc;
	uint64 _totalBytes = 0;

public:
	SD3D12Buffer(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12Resource> resource, ComPtr<ID3D12Resource> uploadHeap, const RHIBufferDesc& desc);

	virtual RHIBufferDesc GetDesc() override { return _desc; }

	virtual void UpdateSubresource(SD3D12CommandList* commandList, uint32 subresource, const RHISubresourceData* uploadData) override;

	static void UpdateSubresource(
		SD3D12CommandList* commandList,
		ID3D12Resource* buf,
		ID3D12Resource* uploadBuf,
		uint32 subresource,
		uint64 totalBytes,
		const RHISubresourceData* uploadData);
};