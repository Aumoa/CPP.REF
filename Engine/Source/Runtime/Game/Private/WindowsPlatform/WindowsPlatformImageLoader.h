// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlatformMisc/IPlatformImageLoader.h"
#include <guiddef.h>

struct IWICImagingFactory;
struct IPlatformImage;

class SWindowsPlatformImageLoader : implements SObject, implements IPlatformImageLoader
{
	GENERATED_BODY(SWindowsPlatformImageLoader)

public:
	ComPtr<IWICImagingFactory> _factory;

public:
	SWindowsPlatformImageLoader();
	virtual ~SWindowsPlatformImageLoader() override;

	virtual IPlatformImage* LoadImageFromFile(const std::filesystem::path& assetPath, ERHIPixelFormat desiredPixelFormat) override;

private:
	const GUID& GetPixelFormatGUID(ERHIPixelFormat pixelFormat) const;
};