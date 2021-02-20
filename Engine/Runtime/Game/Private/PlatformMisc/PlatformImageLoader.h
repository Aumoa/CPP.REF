// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "WindowsMinimal.h"

class PlatformBitmapFrame;

class PlatformImageLoader : virtual public Object
{
public:
	using Super = Object;

private:
	uint8 bValid : 1;
	ComPtr<IWICBitmapDecoder> decoder;
	size_t frameCount;

public:
	PlatformImageLoader(TRefPtr<String> inFilepath);
	~PlatformImageLoader();

	TRefPtr<PlatformBitmapFrame> GetFrame(size_t frameIndex) const;

	vs_property_get_auto(bool, IsValid, bValid);
	vs_property_get_auto(size_t, FrameCount, frameCount);

	static void Initialize();
	static void Shutdown();
};