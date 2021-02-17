// Copyright 2020 Aumoa.lib. All right reserved.

#include "PlatformImage.h"
#include "PlatformImageCommon.inl"

using namespace std;

PlatformImage::PlatformImage(IWICBitmapSource* inSourceWeak) : Super()
	, sourceWeak(inSourceWeak)
	, width(0)
	, height(0)
	, bitsPerPixel(0)
{
	sourceWeak->GetSize(&width, &height);

	WICPixelFormatGUID format;
	sourceWeak->GetPixelFormat(&format);
	bitsPerPixel = CalcBitsPerPixel(format);
}

PlatformImage::~PlatformImage()
{

}

void PlatformImage::CopyPixels(optional<ImageRect> inCopyRect, size_t rowStride, size_t bufferSize, int8* buffer) const
{
	WICRect* wicRect = inCopyRect.has_value() ? (WICRect*)&inCopyRect.value() : nullptr;
	if (FAILED(sourceWeak->CopyPixels(wicRect, (UINT)rowStride, (UINT)bufferSize, (BYTE*)buffer)))
	{
		SE_LOG(LogPlatform, Error, L"CopyPixels: Cannot copy to buffer.");
		return;
	}
}

void PlatformImage::CopyPixels(size_t rowStride, size_t bufferSize, int8* buffer) const
{
	CopyPixels(nullopt, rowStride, bufferSize, buffer);
}

void PlatformImage::CopyPixels(size_t rowStride, int8* buffer) const
{
	size_t bytes = BitsPerPixel / 8;
	size_t bufferSize = rowStride * (Height - 1) + Width * bytes;
	CopyPixels(rowStride, bufferSize, buffer);
}