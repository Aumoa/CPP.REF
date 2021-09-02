// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIEnums.h"

struct RENDERCORE_API IRHIImageSourceView : virtual public Object
{
	virtual void CopyPixels(uint32 stride, uint32 bufferSize, void* pBuffer) = 0;
	virtual int32 GetWidth() const = 0;
	virtual int32 GetHeight() const = 0;
	virtual ERHIPixelFormat GetPixelFormat() const = 0;
};