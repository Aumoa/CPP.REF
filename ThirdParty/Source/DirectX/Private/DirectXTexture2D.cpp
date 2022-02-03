// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXTexture2D.h"

GENERATE_BODY(SDirectXTexture2D);

SDirectXTexture2D::SDirectXTexture2D(IRHIDevice* Owner, ComPtr<ID3D12Resource> pResource)
	: Super(Owner, std::move(pResource))
{
}

Vector2N SDirectXTexture2D::GetPixelSize()
{
	D3D12_RESOURCE_DESC Desc = pResource->GetDesc();
	return Vector2N((int32)Desc.Width, (int32)Desc.Height);
}