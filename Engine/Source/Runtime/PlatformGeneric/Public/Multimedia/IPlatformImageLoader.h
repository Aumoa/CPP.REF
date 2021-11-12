// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIEnums.h"

interface IPlatformImage;

interface IPlatformImageLoader : implements SObject
{
	virtual IPlatformImage* CreateImageFromFile(const std::filesystem::path& InAssetPath, int32 FrameIndex, ERHIPixelFormat PixelFormat) = 0;
};