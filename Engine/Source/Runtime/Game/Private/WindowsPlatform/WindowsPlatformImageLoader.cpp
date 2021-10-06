// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "WindowsPlatformImageLoader.h"
#include "WindowsPlatformHelper.h"
#include "WindowsPlatformImage.h"

SWindowsPlatformImageLoader::SWindowsPlatformImageLoader() : Super()
{
	// Initialize COM subsystem.
	HR(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
	HR(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_factory)));
}

SWindowsPlatformImageLoader::~SWindowsPlatformImageLoader()
{
}

IPlatformImage* SWindowsPlatformImageLoader::LoadImageFromFile(const std::filesystem::path& assetPath, ERHIPixelFormat desiredPixelFormat)
{
	ComPtr<IWICBitmapDecoder> decoder;
	HR(_factory->CreateDecoderFromFilename(assetPath.wstring().c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder));
	
	ComPtr<IWICBitmapFrameDecode> frame;
	HR(decoder->GetFrame(0, &frame));

	ComPtr<IWICFormatConverter> converter;
	HR(_factory->CreateFormatConverter(&converter));
	HR(converter->Initialize(frame.Get(), GetPixelFormatGUID(desiredPixelFormat), WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom));

	return NewObject<SWindowsPlatformImage>(converter.Get(), desiredPixelFormat);
}

const GUID& SWindowsPlatformImageLoader::GetPixelFormatGUID(ERHIPixelFormat pixelFormat) const
{
	switch (pixelFormat)
	{
	case ERHIPixelFormat::B8G8R8A8_UNORM:
		return GUID_WICPixelFormat32bppPBGRA;
	case ERHIPixelFormat::R8G8B8A8_UNORM:
		return GUID_WICPixelFormat32bppPRGBA;
	default:
		SE_LOG(LogWindows, Fatal, L"Not supported pixel format.");
		throw;
	}
}

IPlatformImageLoader& IPlatformImageLoader::Get()
{
	static SWindowsPlatformImageLoader loader;
	return loader;
}