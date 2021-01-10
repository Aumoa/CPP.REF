// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12Resource.h"

D3D12Resource::D3D12Resource(ID3D12Resource* resource) : Super()
	, mappingAddr(nullptr)
{
	this->resource = resource;
}

D3D12Resource::~D3D12Resource()
{

}

uint64 D3D12Resource::GetVirtualAddress() const
{
	return resource->GetGPUVirtualAddress();
}

void* D3D12Resource::GetMappingAddress() const
{
	return mappingAddr;
}

void D3D12Resource::BindMappingAddress()
{
	HR(resource->Map(0, nullptr, &mappingAddr));
}

ID3D12Resource* D3D12Resource::Resource_get() const
{
	return resource.Get();
}