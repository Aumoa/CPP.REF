// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Resource.h"

GENERATE_BODY(SD3D12Resource);

SD3D12Resource::SD3D12Resource(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12Resource> resource, ComPtr<ID3D12Resource> uploadBuf) : Super(InFactory, InDevice, resource.Get())
	, _resource(std::move(resource))
{
}

uint64 SD3D12Resource::GetGPUVirtualAddress()
{
	return _resource->GetGPUVirtualAddress();
}