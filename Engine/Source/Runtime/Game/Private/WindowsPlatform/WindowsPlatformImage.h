// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlatformMisc/IPlatformImage.h"

struct IWICFormatConverter;

class WindowsPlatformImage : virtual public Object, virtual public IPlatformImage
{
	GENERATED_BODY(WindowsPlatformImage)

private:
	ComPtr<IWICFormatConverter> _imageSource = nullptr;
	int32 _width = 0;
	int32 _height = 0;
	ERHIPixelFormat _format = ERHIPixelFormat::Unknown;

public:
	WindowsPlatformImage(IWICFormatConverter* imageSource, ERHIPixelFormat format);
	virtual ~WindowsPlatformImage() override;

	virtual void CopyPixels(uint32 stride, uint32 bufferSize, void* pBuffer) override;
	virtual int32 GetWidth() const override;
	virtual int32 GetHeight() const override;
	virtual ERHIPixelFormat GetPixelFormat() const override;
};