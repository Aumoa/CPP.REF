// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "GenericPlatform/GenericImage.h"

#if PLATFORM_WINDOWS

#include "WindowsCommon.h"
#include "WindowsCo.h"

struct NGenericImage::Impl
{
	ComPtr<IWICFormatConverter> pConverter;
};

NGenericImage::NGenericImage()
{
}

NGenericImage::~NGenericImage() noexcept
{
}

Task<std::shared_ptr<NGenericImage>> NGenericImage::LoadFromFileAsync(String InFilename)
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

		auto Ptr = std::shared_ptr<NGenericImage>(new NGenericImage());
		Ptr->PlatformImpl = std::unique_ptr<Impl>(new Impl());
		Ptr->PlatformImpl->pConverter = std::move(pConverter);

		return Ptr;
	});
}

#endif