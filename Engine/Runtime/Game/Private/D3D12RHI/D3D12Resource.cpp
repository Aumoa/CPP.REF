// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12Resource.h"

D3D12Resource::D3D12Resource(ID3D12Resource* inResource) : Super()
	, mappingAddr(nullptr)
	, virtualAddress(0)
{
	resource = inResource;
	UpdateResourceDesc();
	BindMappingAddress();
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

RHIResourceDesc D3D12Resource::GetDesc() const
{
	return desc;
}

void D3D12Resource::BindMappingAddress()
{
	D3D12_HEAP_PROPERTIES heapProp;
	D3D12_HEAP_FLAGS flags;
	HR(resource->GetHeapProperties(&heapProp, &flags));

	if (heapProp.Type == D3D12_HEAP_TYPE_UPLOAD) {
		HR(resource->Map(0, nullptr, &mappingAddr));
	}
}

ID3D12Resource* D3D12Resource::Resource_get() const
{
	return resource.Get();
}

void D3D12Resource::UpdateResourceDesc()
{
	D3D12_RESOURCE_DESC desc = resource->GetDesc();
	this->desc.Format = (ERHITextureFormat)desc.Format;
	this->desc.Width = desc.Width;
	this->desc.Height = desc.Height;
	this->desc.DepthOrArraySize = desc.DepthOrArraySize;
}