// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "WindowsPlatformImage.h"
#include "WindowsPlatformHelper.h"
#include <wincodec.h>

SWindowsPlatformImage::SWindowsPlatformImage(IWICFormatConverter* imageSource, ERHIPixelFormat format) : Super()
	, _imageSource(imageSource)
	, _format(format)
{
	UINT width, height;
	HR(_imageSource->GetSize(&width, &height));
	_width = (int32)width;
	_height = (int32)height;
}

SWindowsPlatformImage::~SWindowsPlatformImage()
{
}

void SWindowsPlatformImage::CopyPixels(uint32 stride, uint32 bufferSize, void* pBuffer)
{
	HR(_imageSource->CopyPixels(nullptr, stride, bufferSize, (BYTE*)pBuffer));
}

int32 SWindowsPlatformImage::GetWidth() const
{
	return _width;
}

int32 SWindowsPlatformImage::GetHeight() const
{
	return _height;
}

ERHIPixelFormat SWindowsPlatformImage::GetPixelFormat() const
{
	return _format;
}