// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "RHIResource.h"
#include <d3d12.h>
#include "LogRHI.h"

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