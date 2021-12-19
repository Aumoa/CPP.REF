// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIEnums.h"
#include <filesystem>

interface IPlatformImage;

interface IPlatformImageLoader : implements SObject
{
	GENERATED_INTERFACE_BODY(IPlatformImageLoader)

	virtual IPlatformImage* CreateImageFromFile(const std::filesystem::path& InAssetPath, int32 FrameIndex, ERHIPixelFormat PixelFormat) = 0;
	virtual IPlatformImage* CreateImageFromBinary(std::span<const uint8> AssetsBin, int32 FrameIndex, ERHIPixelFormat PixelFormat) = 0;
};