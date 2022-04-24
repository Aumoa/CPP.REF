// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "WindowsImage.h"

using namespace ::libty;

SWindowsImage::SWindowsImage(IWICBitmapSource* ImageSource, ERHIPixelFormat Format) : Super()
	, ImageSource(ImageSource)
	, Format(Format)
{
}

void SWindowsImage::Dispose()
{
	this->Dispose(true);
}

void SWindowsImage::CopyPixels(const RectN* CopyRect, int32 Stride, int32 BufferSize, void* OutBuffer)
{
	WICRect WRect;
	if (CopyRect)
	{
		WRect.X = CopyRect->Left;
		WRect.Y = CopyRect->Top;
		WRect.Width = Rect::Width(*CopyRect);
		WRect.Height = Rect::Height(*CopyRect);
	}

	HR(ImageSource->CopyPixels(CopyRect ? &WRect : nullptr, (UINT)Stride, (UINT)BufferSize, (BYTE*)OutBuffer));
}

Vector2N SWindowsImage::GetPixelSize()
{
	Vector2N Sz;
	HR(ImageSource->GetSize((UINT*)&Sz.X, (UINT*)&Sz.Y));
	return Sz;
}

ERHIPixelFormat SWindowsImage::GetPixelFormat()
{
	return Format;
}

void SWindowsImage::Dispose(bool bDisposing)
{
	ImageSource.Reset();
	Super::Dispose(bDisposing);
}