// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlatformMisc/IPlatformImageLoader.h"

struct IWICImagingFactory;
struct IPlatformImage;

class WindowsPlatformImageLoader : virtual public Object, virtual public IPlatformImageLoader
{
	CLASS_BODY(WindowsPlatformImageLoader)

public:
	ComPtr<IWICImagingFactory> _factory;

public:
	WindowsPlatformImageLoader();
	virtual ~WindowsPlatformImageLoader() override;

	virtual IPlatformImage* LoadImageFromFile(const std::filesystem::path& assetPath, ERHIPixelFormat desiredPixelFormat) override;

private:
	const GUID& GetPixelFormatGUID(ERHIPixelFormat pixelFormat) const;
};