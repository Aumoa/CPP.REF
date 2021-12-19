// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIDeviceChild.h"
#include "RHIEnums.h"

interface IRHIBrush;
interface IRHIImage;
interface IRHIBitmap;
interface IRHITextLayout;

interface IRHIDeviceContext2D : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIDeviceContext2D)

	virtual void BeginDraw() = 0;
	virtual void EndDraw() = 0;
	virtual void SetTarget(IRHIImage* InTarget, std::optional<Color> ClearColor = std::nullopt) = 0;
	virtual void SetTransform(const Matrix3x2& Transform) = 0;
	virtual Matrix3x2 GetTransform() = 0;
	virtual void FillRectangle(IRHIBrush* InBrush, const Rect& Region) = 0;
	virtual void FillEllipse(IRHIBrush* InBrush, const Rect& Region) = 0;
	virtual void DrawTextLayout(const Vector2& DrawOrigin, IRHITextLayout* Layout, IRHIBrush* InBrush, ERHIDrawTextOptions Options = ERHIDrawTextOptions::None) = 0;
	virtual void DrawImage(IRHIImage* Image, const Vector2* Offset, const Rect* SourceRect, ERHIInterpolationMode InterpolationMode = ERHIInterpolationMode::NearestNeighbor, ERHICompositeMode CompositeMode = ERHICompositeMode::SourceOver) = 0;
	virtual void DrawBitmap(IRHIBitmap* Bitmap, const Rect* DestRect, float Opacity = 1.0f, ERHIInterpolationMode InterpolationMode = ERHIInterpolationMode::NearestNeighbor, const Rect* SourceRect = nullptr, const Matrix4x4* PerspectiveTransform = nullptr) = 0;
};