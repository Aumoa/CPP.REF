// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D2D1Bitmap.h"

GENERATE_BODY(SD2D1Bitmap);

SD2D1Bitmap::SD2D1Bitmap(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID2D1Bitmap1* Bitmap) : Super(InFactory, InDevice, Bitmap)
	, Bitmap(Bitmap)
{
}

Vector2N SD2D1Bitmap::GetSize()
{
	D2D1_SIZE_U Size = Bitmap->GetPixelSize();
	return (Vector2N&)Size;
}

void SD2D1Bitmap::InitWrappedResources(ID3D12Resource* pResource, ID3D11Resource* pWrapped)
{
	Resource = pResource;
	WrappedResource = pWrapped;
}

bool SD2D1Bitmap::IsMultisampled()
{
	if (Resource)
	{
		D3D12_RESOURCE_DESC Desc = Resource->GetDesc();
		return Desc.SampleDesc.Count != 1;
	}
	return false;
}