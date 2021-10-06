// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlatformMisc/IPlatformImage.h"
#include "WindowsPlatformHelper.h"

class SWindowsPlatformImage : implements SObject, implements IPlatformImage
{
	GENERATED_BODY(SWindowsPlatformImage)

private:
	ComPtr<IWICFormatConverter> _imageSource = nullptr;
	int32 _width = 0;
	int32 _height = 0;
	ERHIPixelFormat _format = ERHIPixelFormat::Unknown;

public:
	SWindowsPlatformImage(IWICFormatConverter* imageSource, ERHIPixelFormat format);
	virtual ~SWindowsPlatformImage() override;

	virtual void CopyPixels(uint32 stride, uint32 bufferSize, void* pBuffer) override;
	Vector2N GetPixelSize() const override;
	virtual ERHIPixelFormat GetPixelFormat() const override;
};