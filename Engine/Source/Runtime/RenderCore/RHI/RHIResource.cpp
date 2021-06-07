// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Internal.h"

import SC.Runtime.RenderCore;

RHIResource::RHIResource(RHIDevice* device, ID3D12Resource* resource) : Super(device)
	, _resource(resource)
{
}

RHIResource::~RHIResource()
{
}