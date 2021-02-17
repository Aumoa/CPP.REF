// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "PlatformImage.h"

enum class ERHITextureFormat;

class PlatformBitmapFrame : public PlatformImage
{
public:
	using Super = PlatformImage;

private:
	ComPtr<IWICBitmapFrameDecode> frame;

public:
	PlatformBitmapFrame(ComPtr<IWICBitmapFrameDecode> frame);
	~PlatformBitmapFrame() override;

	TRefPtr<PlatformImage> FormatConvert(ERHITextureFormat inDesiredFormat) const;
};