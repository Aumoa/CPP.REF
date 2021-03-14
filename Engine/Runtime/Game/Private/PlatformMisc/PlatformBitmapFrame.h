// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "PlatformImage.h"

#include "COM/TComPtr.h"

enum class ERHITextureFormat;

class PlatformBitmapFrame : public PlatformImage
{
public:
	using Super = PlatformImage;

private:
	TComPtr<IWICBitmapFrameDecode> frame;

public:
	PlatformBitmapFrame(TComPtr<IWICBitmapFrameDecode> frame);
	~PlatformBitmapFrame() override;

	TRefPtr<PlatformImage> FormatConvert(ERHITextureFormat inDesiredFormat) const;
};