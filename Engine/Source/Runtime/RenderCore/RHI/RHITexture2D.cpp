// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Internal.h"

import SC.Runtime.RenderCore;

RHITexture2D::RHITexture2D(RHIDevice* device, ID3D12Resource* resource) : Super(device, resource)
{
}

RHITexture2D::~RHITexture2D()
{
}