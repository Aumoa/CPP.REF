// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "COM/COMImageLoader.h"
#include "COMImageCommon.inl"

#include "COM/COMBitmapFrame.h"
#include "COM/COMDeviceBundle.h"
#undef interface
#include <wincodec.h>

using namespace std;

COMImageLoader::COMImageLoader(COMDeviceBundle* deviceBundle, TRefPtr<String> inFilepath) : Super()
	, deviceBundle(deviceBundle)
	, bValid(false)
	, frameCount(0)
{
	IWICImagingFactory* imagingFactory = deviceBundle->GetImagingFactory();

	if (FAILED(imagingFactory->CreateDecoderFromFilename(inFilepath->C_Str, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder)))
	{
		SE_LOG(LogCOM, Error, L"Failed to load decoder.");
		return;
	}

	UINT frameCount;
	if (FAILED(decoder->GetFrameCount(&frameCount)))
	{
		SE_LOG(LogCOM, Error, L"Failed to query image metadata. Image is corrupted.");
		return;
	}

	this->frameCount = frameCount;
	bValid = true;
}

COMImageLoader::~COMImageLoader()
{

}

TRefPtr<COMBitmapFrame> COMImageLoader::GetFrame(size_t frameIndex) const
{
	if (frameIndex > frameCount)
	{
		SE_LOG(LogCOM, Error, L"In frame index({0}) cannot be over than frame count({1}).", frameIndex, frameCount);
		return nullptr;
	}

	TComPtr<IWICBitmapFrameDecode> frame;
	if (FAILED(decoder->GetFrame((UINT)frameIndex, &frame)))
	{
		SE_LOG(LogCOM, Error, L"Cannot query bitmap frame. Bitmap or frame is corrupted.");
		return nullptr;
	}

	return NewObject<COMBitmapFrame>(deviceBundle, move(frame));
}