// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Buffer.h"

SD3D12Buffer::SD3D12Buffer(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12Resource> resource, ComPtr<ID3D12Resource> uploadHeap) : Super(factory, device, resource)
	, _resource(std::move(resource))
	, _uploadHeap(std::move(uploadHeap))
{
}