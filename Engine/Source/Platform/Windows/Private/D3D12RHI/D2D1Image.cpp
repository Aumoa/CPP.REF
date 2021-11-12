// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D2D1Image.h"

SD2D1Image::SD2D1Image(SDXGIFactory* Factory, SD3D12Device* Device, ID2D1Image* Image) : Super(Factory, Device, nullptr)
	, ImageSource(Image)
{
}