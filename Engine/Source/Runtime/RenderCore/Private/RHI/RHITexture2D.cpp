// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHI/RHITexture2D.h"

SRHITexture2D::SRHITexture2D(SRHIDevice* device, ID3D12Resource* resource) : Super(device, resource)
{
}

SRHITexture2D::~SRHITexture2D()
{
}

Vector2N SRHITexture2D::GetPixelSize() const
{
	D3D12_RESOURCE_DESC desc = GetResource()->GetDesc();
	return Vector2N((int32)desc.Width, (int32)desc.Height);
}