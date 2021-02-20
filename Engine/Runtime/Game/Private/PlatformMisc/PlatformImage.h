// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "WindowsMinimal.h"

class PlatformImage : virtual public Object
{
public:
	using Super = Object;

	struct ImageRect
	{
		int32 X;
		int32 Y;
		int32 Width;
		int32 Height;
	};

private:
	IWICBitmapSource* sourceWeak;
	uint32 width;
	uint32 height;
	size_t bitsPerPixel;

public:
	PlatformImage(IWICBitmapSource* inSourceWeak);
	~PlatformImage() override;

	virtual void CopyPixels(std::optional<ImageRect> inCopyRect, size_t rowStride, size_t bufferSize, int8* buffer) const;
	void CopyPixels(size_t rowStride, size_t bufferSize, int8* buffer) const;
	void CopyPixels(size_t rowStride, int8* buffer) const;

	vs_property_get_auto(uint32, Width, width);
	vs_property_get_auto(uint32, Height, height);
	vs_property_get_auto(size_t, BitsPerPixel, bitsPerPixel);
};