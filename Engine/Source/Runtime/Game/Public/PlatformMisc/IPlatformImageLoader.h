// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct IPlatformImage;

struct GAME_API IPlatformImageLoader : implements SObject
{
	virtual IPlatformImage* LoadImageFromFile(const std::filesystem::path& assetPath, ERHIPixelFormat desiredPixelFormat) = 0;

	static IPlatformImageLoader& Get();
};