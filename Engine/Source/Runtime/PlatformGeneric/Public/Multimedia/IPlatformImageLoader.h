// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "RHI/RHIEnums.h"
#include <filesystem>

namespace libty::inline PlatformGeneric
{
	struct IPlatformImage;

	struct PLATFORMGENERIC_API IPlatformImageLoader : virtual public SObject
	{
		GENERATED_INTERFACE_BODY(IPlatformImageLoader);

		virtual IPlatformImage* CreateImageFromFile(const std::filesystem::path& InAssetPath, int32 FrameIndex, ERHIPixelFormat PixelFormat) = 0;
		virtual IPlatformImage* CreateImageFromBinary(std::span<const uint8> AssetsBin, int32 FrameIndex, ERHIPixelFormat PixelFormat) = 0;
	};
}