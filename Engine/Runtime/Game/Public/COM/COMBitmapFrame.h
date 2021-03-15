// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "COMImage.h"

#include "COM/TComPtr.h"

class COMDeviceBundle;
enum class ERHITextureFormat;

class COMBitmapFrame : public COMImage
{
public:
	using Super = COMImage;

private:
	COMDeviceBundle* deviceBundle;
	TComPtr<IWICBitmapFrameDecode> frame;

public:
	COMBitmapFrame(COMDeviceBundle* deviceBundle, TComPtr<IWICBitmapFrameDecode> frame);
	~COMBitmapFrame() override;

	TRefPtr<COMImage> ConvertFormat(ERHITextureFormat inDesiredFormat) const;
};