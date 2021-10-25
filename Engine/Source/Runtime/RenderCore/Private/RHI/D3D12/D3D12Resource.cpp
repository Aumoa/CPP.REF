// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Resource.h"

SD3D12Resource::SD3D12Resource(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12Resource> resource, ComPtr<ID3D12Resource> uploadBuf) : Super(factory, device, resource.Get())
	, _resource(std::move(resource))
{
}

uint64 SD3D12Resource::GetGPUVirtualAddress()
{
	return _resource->GetGPUVirtualAddress();
}