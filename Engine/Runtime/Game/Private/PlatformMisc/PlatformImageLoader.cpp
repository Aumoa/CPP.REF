// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "PlatformImageLoader.h"
#include "PlatformImageCommon.inl"

#include "PlatformBitmapFrame.h"

using namespace std;

TComPtr<IWICImagingFactory2> imagingFactory;

PlatformImageLoader::PlatformImageLoader(TRefPtr<String> inFilepath) : Super()
	, bValid(false)
	, frameCount(0)
{
	if (FAILED(imagingFactory->CreateDecoderFromFilename(inFilepath->C_Str, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder)))
	{
		SE_LOG(LogPlatform, Error, L"Failed to load decoder.");
		return;
	}

	UINT frameCount;
	if (FAILED(decoder->GetFrameCount(&frameCount)))
	{
		SE_LOG(LogPlatform, Error, L"Failed to query image metadata. Image is corrupted.");
		return;
	}

	this->frameCount = frameCount;
	bValid = true;
}

PlatformImageLoader::~PlatformImageLoader()
{

}

TRefPtr<PlatformBitmapFrame> PlatformImageLoader::GetFrame(size_t frameIndex) const
{
	if (frameIndex > frameCount)
	{
		SE_LOG(LogPlatform, Error, L"In frame index({0}) cannot be over than frame count({1}).", frameIndex, frameCount);
		return nullptr;
	}

	TComPtr<IWICBitmapFrameDecode> frame;
	if (FAILED(decoder->GetFrame((UINT)frameIndex, &frame)))
	{
		SE_LOG(LogPlatform, Error, L"Cannot query bitmap frame. Bitmap or frame is corrupted.");
		return nullptr;
	}

	return NewObject<PlatformBitmapFrame>(move(frame));
}

void PlatformImageLoader::Initialize()
{
	if (FAILED(::CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		SE_LOG(LogPlatform, Fatal, L"Failed to initialize COM.");
		return;
	}

	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&imagingFactory))))
	{
		SE_LOG(LogPlatform, Fatal, L"Failed to initialize image load engine.");
		return;
	}
}

void PlatformImageLoader::Shutdown()
{
	imagingFactory.Reset();
}