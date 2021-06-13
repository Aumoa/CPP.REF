// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Internal.h"

import SC.Runtime.Core;
import SC.Runtime.RenderCore;

RHITexture::RHITexture(RHIDevice* device, ID3D12Resource* resource) : Super(device, resource)
{
}

RHITexture::~RHITexture()
{
}