// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "COM/COMBitmapFrame.h"
#include "COMImageCommon.inl"

#include "COM/COMDeviceBundle.h"

using namespace std;

COMBitmapFrame::COMBitmapFrame(COMDeviceBundle* deviceBundle, TComPtr<IWICBitmapFrameDecode> frame) : Super(frame.Get())
	, deviceBundle(deviceBundle)
	, frame(move(frame))
{

}

COMBitmapFrame::~COMBitmapFrame()
{

}

TRefPtr<COMImage> COMBitmapFrame::ConvertFormat(ERHITextureFormat inDesiredFormat) const
{
	WICPixelFormatGUID formatGuid = GetWICFormat(inDesiredFormat);
	if (formatGuid == GUID_WICPixelFormatUndefined)
	{
		SE_LOG(LogPlatform, Error, L"Cannot convert to desired pixel format. Abort.");
		return nullptr;
	}

	class PlatformBitmapConverter : public COMImage
	{
	public:
		using Super = COMImage;

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

	IWICImagingFactory* imagingFactory = deviceBundle->GetImagingFactory();

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