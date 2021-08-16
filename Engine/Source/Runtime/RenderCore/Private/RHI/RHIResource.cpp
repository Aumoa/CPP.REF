// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHI/RHIResource.h"
#include "InternalComPtr.h"
#include "RHI/LogRHI.h"

RHIResource::RHIResource(RHIDevice* device, ID3D12Resource* resource) : Super(device)
	, _resource(resource)
{
}

RHIResource::~RHIResource()
{
}

uint64 RHIResource::GetGPUVirtualAddress() const
{
	return _resource->GetGPUVirtualAddress();
}

uint64 RHIResource::GetBufferSize() const
{
	D3D12_RESOURCE_DESC desc = _resource->GetDesc();
	return desc.Width * desc.Height * desc.DepthOrArraySize;
}

void* RHIResource::GetMappingPointer() const
{
	if (_ptr == nullptr)
	{
		// Cache mapping buffer pointer on demand.
		RHIResource* mutableThis = const_cast<RHIResource*>(this);
		mutableThis->Mapping();
	}
	return _ptr;
}

void RHIResource::Mapping()
{
	HR(LogRHI, _resource->Map(0, nullptr, &_ptr));
}