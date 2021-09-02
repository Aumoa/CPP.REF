// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "WindowsPlatformImage.h"
#include "WindowsPlatformHelper.h"
#include <wincodec.h>

WindowsPlatformImage::WindowsPlatformImage(IWICFormatConverter* imageSource, ERHIPixelFormat format) : Super()
	, _imageSource(imageSource)
	, _format(format)
{
	UINT width, height;
	HR(_imageSource->GetSize(&width, &height));
	_width = (int32)width;
	_height = (int32)height;
}

WindowsPlatformImage::~WindowsPlatformImage()
{
}

void WindowsPlatformImage::CopyPixels(uint32 stride, uint32 bufferSize, void* pBuffer)
{
	HR(_imageSource->CopyPixels(nullptr, stride, bufferSize, (BYTE*)pBuffer));
}

int32 WindowsPlatformImage::GetWidth() const
{
	return _width;
}

int32 WindowsPlatformImage::GetHeight() const
{
	return _height;
}

ERHIPixelFormat WindowsPlatformImage::GetPixelFormat() const
{
	return _format;
}