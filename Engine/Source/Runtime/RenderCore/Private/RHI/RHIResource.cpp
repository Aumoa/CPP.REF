// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHI/RHIResource.h"
#include "InternalComPtr.h"
#include "RHI/LogRHI.h"

SRHIResource::SRHIResource(SRHIDevice* device, ID3D12Resource* resource) : Super(device)
	, _resource(resource)
{
}

SRHIResource::~SRHIResource()
{
}

uint64 SRHIResource::GetGPUVirtualAddress() const
{
	return _resource->GetGPUVirtualAddress();
}

uint64 SRHIResource::GetBufferSize() const
{
	D3D12_RESOURCE_DESC desc = _resource->GetDesc();
	return desc.Width * desc.Height * desc.DepthOrArraySize;
}

void* SRHIResource::GetMappingPointer() const
{
	if (_ptr == nullptr)
	{
		// Cache mapping buffer pointer on demand.
		SRHIResource* mutableThis = const_cast<SRHIResource*>(this);
		mutableThis->Mapping();
	}
	return _ptr;
}

void SRHIResource::Mapping()
{
	HR(LogRHI, _resource->Map(0, nullptr, &_ptr));
}