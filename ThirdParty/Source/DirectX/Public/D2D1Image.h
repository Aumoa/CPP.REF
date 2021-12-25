// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "RHI/IRHIImage.h"

class DIRECTX_API SD2D1Image : public SD3D12DeviceChild, implements IRHIImage
{
	GENERATED_BODY(SD2D1Image)

private:
	ComPtr<ID2D1Image> ImageSource;

public:
	SD2D1Image(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID2D1Image* Image);

public:
	DECLARE_GETTER(ID2D1Image, ImageSource);
};