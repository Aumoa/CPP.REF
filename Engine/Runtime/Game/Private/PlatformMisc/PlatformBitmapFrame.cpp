// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "PlatformBitmapFrame.h"
#include "PlatformImageCommon.inl"

using namespace std;

PlatformBitmapFrame::PlatformBitmapFrame(TComPtr<IWICBitmapFrameDecode> frame) : Super(frame.Get())
	, frame(move(frame))
{

}

PlatformBitmapFrame::~PlatformBitmapFrame()
{

}

TRefPtr<PlatformImage> PlatformBitmapFrame::FormatConvert(ERHITextureFormat inDesiredFormat) const
{
	WICPixelFormatGUID formatGuid = GetWICFormat(inDesiredFormat);
	if (formatGuid == GUID_WICPixelFormatUndefined)
	{
		SE_LOG(LogPlatform, Error, L"Cannot convert to desired pixel format. Abort.");
		return nullptr;
	}

	class PlatformBitmapConverter : public PlatformImage
	{
	public:
		using Super = PlatformImage;

	private:
		TComPtr<IWICFormatConverter> converter;

	public:
		PlatformBitmapConverter(TComPtr<IWICFormatConverter> converter) : Super(converter.Get())
			, converter(move(converter))
		{

		}

		~PlatformBitmapConverter() override
		{

		}
	};

	TComPtr<IWICFormatConverter> converter;
	if (FAILED(imagingFactory->CreateFormatConverter(&converter)))
	{
		SE_LOG(LogPlatform, Fatal, L"Cannot create format converter. Factory was corrupted.");
		return nullptr;
	}

	if (FAILED(converter->Initialize(frame.Get(), formatGuid, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom)))
	{
		SE_LOG(LogPlatform, Error, L"Cannot convert to desired pixel format. Abort.");
		return nullptr;
	}

	return NewObject<PlatformBitmapConverter>(move(converter));
}