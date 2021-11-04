// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIDeviceChild.h"
#include "RHIEnums.h"

interface IRHIBrush;
interface IRHITexture2D;
interface IRHITextLayout;

interface IRHIDeviceContext2D : implements IRHIDeviceChild
{
	virtual void BeginDraw() = 0;
	virtual void EndDraw() = 0;
	virtual void SetTarget(IRHITexture2D* InTarget, std::optional<Color> ClearColor = std::nullopt) = 0;
	virtual void FillRectangle(IRHIBrush* InBrush, const Rect& Region) = 0;
	virtual void DrawTextLayout(const Vector2& DrawOrigin, IRHITextLayout* Layout, IRHIBrush* InBrush, ERHIDrawTextOptions Options = ERHIDrawTextOptions::None) = 0;
};