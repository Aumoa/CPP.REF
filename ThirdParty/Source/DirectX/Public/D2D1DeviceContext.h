// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "RHI/IRHIDeviceContext2D.h"

interface IRHIImage;

class DIRECTX_API SD2D1DeviceContext : public SD3D12DeviceChild, implements IRHIDeviceContext2D
{
	GENERATED_BODY(SD2D1DeviceContext)

private:
	SPROPERTY(InteropDev)
	ID3D11On12Device* InteropDev = nullptr;
	SPROPERTY(Target)
	IRHIImage* Target = nullptr;

	ComPtr<ID2D1DeviceContext> DeviceContext;

public:
	SD2D1DeviceContext(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID2D1DeviceContext* InDeviceContext, ID3D11On12Device* InteropDevice);

	virtual void BeginDraw() override;
	virtual void EndDraw() override;
	virtual void SetTarget(IRHIImage* InTarget, std::optional<Color> ClearColor) override;
	virtual void SetTransform(const Matrix3x2& Transform) override;
	virtual Matrix3x2 GetTransform() override;
	virtual void FillRectangle(IRHIBrush* InBrush, const Rect& Region) override;
	virtual void FillEllipse(IRHIBrush* InBrush, const Rect& Region) override;
	virtual void DrawTextLayout(const Vector2& DrawOrigin, IRHITextLayout* Layout, IRHIBrush* InBrush, ERHIDrawTextOptions Options = ERHIDrawTextOptions::None) override;
	virtual void DrawImage(IRHIImage* Image, const Vector2* Offset, const Rect* SourceRect, ERHIInterpolationMode InterpolationMode = ERHIInterpolationMode::NearestNeighbor, ERHICompositeMode CompositeMode = ERHICompositeMode::SourceOver) override;
	virtual void DrawBitmap(IRHIBitmap* Bitmap, const Rect* DestRect, float Opacity = 1.0f, ERHIInterpolationMode InterpolationMode = ERHIInterpolationMode::NearestNeighbor, const Rect* SourceRect = nullptr, const Matrix4x4* PerspectiveTransform = nullptr) override;

private:
	void AcquireWrappedTarget();
	void ReleaseWrappedTarget();
};