// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIDeviceChild.h"

interface IRHIBrush;
interface IRHITexture2D;

interface IRHIDeviceContext2D : implements IRHIDeviceChild
{
	virtual void BeginDraw() = 0;
	virtual void EndDraw() = 0;
	virtual void SetTarget(IRHITexture2D* InTarget) = 0;
	virtual void FillRectangle(IRHIBrush* InBrush, const Rect& Region) = 0;
};