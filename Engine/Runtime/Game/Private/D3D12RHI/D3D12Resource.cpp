// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12Resource.h"

D3D12Resource::D3D12Resource(ID3D12Resource* resource) : Super()
{
	this->resource = resource;
}

D3D12Resource::~D3D12Resource()
{

}

ID3D12Resource* D3D12Resource::Resource_get() const
{
	return resource.Get();
}