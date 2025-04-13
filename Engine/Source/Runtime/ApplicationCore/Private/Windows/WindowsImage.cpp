// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GenericPlatform/GenericImage.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__
#include "Platform/PlatformCommon.h"
#include "WindowsCo.h"

#define IMPLV (*(IWICFormatConverter*)pImpl)

namespace Ayla
{
	GenericImage::GenericImage()
	{
	}

	GenericImage::~GenericImage() noexcept
	{
		if (pImpl)
		{
			IMPLV.Release();
		}
	}

	void GenericImage::CopyPixels(int32 Stride, int32 BufferSize, void* DestBuf)
	{
		check(pImpl);
		HR(IMPLV.CopyPixels(nullptr, (UINT)Stride, (UINT)BufferSize, (BYTE*)DestBuf));
	}

	Task<> GenericImage::CopyPixelsAsync(int32 Stride, int32 BufferSize, void* DestBuf)
	{
		check(pImpl);
		return Task<>::Run([Ptr = ComPtr<IWICFormatConverter>(&IMPLV), Stride, BufferSize, DestBuf]()
			{
				HR(Ptr->CopyPixels(nullptr, (UINT)Stride, (UINT)BufferSize, (BYTE*)DestBuf));
			});
	}

	void GenericImage::CopyPixels(const RectN& InCopyRect, int32 Stride, int32 BufferSize, void* DestBuf)
	{
		check(pImpl);
		WICRect WRect =
		{
			.X = InCopyRect.Left,
			.Y = InCopyRect.Top,
			.Width = Rect::Width(InCopyRect),
			.Height = Rect::Height(InCopyRect)
		};
		HR(IMPLV.CopyPixels(&WRect, (UINT)Stride, (UINT)BufferSize, (BYTE*)DestBuf));
	}

	Task<> GenericImage::CopyPixelsAsync(RectN InCopyRect, int32 Stride, int32 BufferSize, void* DestBuf)
	{
		check(pImpl);
		return Task<>::Run([Ptr = ComPtr<IWICFormatConverter>(&IMPLV), InCopyRect, Stride, BufferSize, DestBuf]()
			{
				WICRect WRect =
				{
					.X = InCopyRect.Left,
					.Y = InCopyRect.Top,
					.Width = Rect::Width(InCopyRect),
					.Height = Rect::Height(InCopyRect)
				};
				HR(Ptr->CopyPixels(&WRect, (UINT)Stride, (UINT)BufferSize, (BYTE*)DestBuf));
			});
	}

	Vector2N GenericImage::GetSize() const
	{
		check(pImpl);
		UINT iWidth, iHeight;
		HR(IMPLV.GetSize(&iWidth, &iHeight));
		return Vector2N((int32)iWidth, (int32)iHeight);
	}

	Task<RPtr<GenericImage>> GenericImage::LoadFromFileAsync(String InFilename)
	{
		return Task<>::Run([pFactory = NWindowsCoStatics::GetImagingFactory(), InFilename]()
		{
			ComPtr<IWICBitmapDecoder> pDecoder;
			HR(pFactory->CreateDecoderFromFilename(InFilename.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder));

			ComPtr<IWICBitmapFrameDecode> pDecodedFrame;
			HR(pDecoder->GetFrame(0, &pDecodedFrame));

			ComPtr<IWICFormatConverter> pConverter;
			HR(pFactory->CreateFormatConverter(&pConverter));
			HR(pConverter->Initialize(pDecodedFrame.Get(), GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom));

			auto Ptr = New<GenericImage>();
			Ptr->pImpl = pConverter.Detach();

			return Ptr;
		});
	}
}

#undef IMPLV
#undef __ALLOW_PLATFORM_COMMON_H__

#endif