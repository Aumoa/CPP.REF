// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIEnums.h"

interface IPlatformImage : implements SObject
{
	GENERATED_INTERFACE_BODY(IPlatformImage)

	virtual void CopyPixels(const RectN* CopyRect, int32 Stride, int32 BufferSize, void* OutBuffer) = 0;
	virtual Vector2N GetPixelSize() = 0;
	virtual ERHIPixelFormat GetPixelFormat() = 0;
};