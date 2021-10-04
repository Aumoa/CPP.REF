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

public:
	SD3D12Buffer(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12Resource> resource, ComPtr<ID3D12Resource> uploadHeap, const RHIBufferDesc& desc);

	virtual RHIBufferDesc GetDesc() override { return _desc; }
};