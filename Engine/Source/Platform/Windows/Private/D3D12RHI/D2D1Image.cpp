// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D2D1Image.h"

GENERATE_BODY(SD2D1Image);

SD2D1Image::SD2D1Image(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID2D1Image* Image) : Super(InFactory, InDevice, nullptr)
	, ImageSource(Image)
{
}