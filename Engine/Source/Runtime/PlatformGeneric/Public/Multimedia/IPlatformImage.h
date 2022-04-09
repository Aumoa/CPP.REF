// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "RHI/RHIEnums.h"

namespace libty::inline PlatformGeneric
{
	struct PLATFORMGENERIC_API IPlatformImage : implements(IDisposable)
	{
		GENERATED_BODY(IPlatformImage);

		virtual void CopyPixels(const RectN* CopyRect, int32 Stride, int32 BufferSize, void* OutBuffer) = 0;
		virtual Vector2N GetPixelSize() = 0;
		virtual ERHIPixelFormat GetPixelFormat() = 0;
	};
}