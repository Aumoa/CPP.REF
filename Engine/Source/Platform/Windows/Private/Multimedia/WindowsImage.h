// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Multimedia/IPlatformImage.h"
#include "WindowsIncludes.h"

class SWindowsImage : implements SObject, implements IPlatformImage
{
	GENERATED_BODY(SWindowsImage)

private:
	ComPtr<IWICBitmapSource> ImageSource;
	ERHIPixelFormat Format;

public:
	SWindowsImage(IWICBitmapSource* ImageSource, ERHIPixelFormat Format);

	virtual void CopyPixels(const RectN* CopyRect, int32 Stride, int32 BufferSize, void* OutBuffer) override;
	virtual Vector2N GetPixelSize() override;
	virtual ERHIPixelFormat GetPixelFormat() override;
};