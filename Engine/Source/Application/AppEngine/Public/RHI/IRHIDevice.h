// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Layout.h"
#include "RHIStructures.h"

interface IRHIDeviceContext;
interface IRHIBrush;
interface IRHISolidColorBrush;
interface IRHIStrokeStyle;

interface IRHIDevice : implements SObject
{
	virtual void Present() = 0;
	virtual void ResizeBuffers(const Vector2N& InPixelSize) = 0;

	virtual IRHISolidColorBrush* CreateSolidColorBrush(const Color& InColor, float InOpacity = 1.0f, const SlateRenderTransform& InTransform = SlateRenderTransform::Identity()) = 0;
	virtual IRHIStrokeStyle* CreateStrokeStyle(const RHIStrokeStyleDesc& Desc, const std::vector<float>& CustomDashes = {}) = 0;

	virtual void BeginDraw(const Color& ClearColor) = 0;
	virtual void EndDraw() = 0;
	virtual void FillRectangle(IRHIBrush* InBrush, Rect InRect) = 0;
	virtual void DrawRectangle(IRHIBrush* InBrush, Rect InRect, float StrokeWidth = 1.0f, IRHIStrokeStyle* StrokeStyle = nullptr, ERHIStrokeDirection Direction = ERHIStrokeDirection::Inner) = 0;
	virtual void SetTransform(const SlateRenderTransform& InTransform) = 0;
	virtual SlateRenderTransform GetTransform() = 0;
};