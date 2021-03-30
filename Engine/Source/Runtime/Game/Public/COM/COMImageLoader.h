// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Windows/WindowsMinimal.h"
#include "COM/TComPtr.h"

class COMDeviceBundle;
class COMBitmapFrame;
interface IWICBitmapDecoder;

class COMImageLoader : virtual public Object
{
public:
	using Super = Object;

private:
	COMDeviceBundle* deviceBundle;
	uint8 bValid : 1;
	TComPtr<IWICBitmapDecoder> decoder;
	size_t frameCount;

public:
	COMImageLoader(COMDeviceBundle* deviceBundle, TRefPtr<String> filepath);
	~COMImageLoader();

	TRefPtr<COMBitmapFrame> GetFrame(size_t frameIndex) const;

	vs_property_get_auto(bool, IsValid, bValid);
	vs_property_get_auto(size_t, FrameCount, frameCount);
};