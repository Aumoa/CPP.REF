// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIEnums.h"

interface IRHIImageSourceView : implements SObject
{
	virtual void CopyPixels(uint32 stride, uint32 bufferSize, void* pBuffer) = 0;
	virtual Vector2N GetPixelSize() const = 0;
	virtual ERHIPixelFormat GetPixelFormat() const = 0;
};