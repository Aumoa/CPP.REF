// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "RHI/IRHIDeviceContext2D.h"

class SD2D1DeviceContext : public SD3D12DeviceChild, implements IRHIDeviceContext2D
{
	GENERATED_BODY(SD2D1DeviceContext)

private:
	ID3D11On12Device* InteropDev = nullptr;
	ComPtr<ID2D1DeviceContext> DeviceContext;
	std::weak_ptr<SObject> WeakTarget;

public:
	SD2D1DeviceContext(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID2D1DeviceContext* InDeviceContext, ID3D11On12Device* InteropDevice);

	virtual void BeginDraw() override;
	virtual void EndDraw() override;
	virtual void SetTarget(IRHITexture2D* InTarget, std::optional<Color> ClearColor) override;
	virtual void FillRectangle(IRHIBrush* InBrush, const Rect& Region) override;
	virtual void DrawTextLayout(const Vector2& DrawOrigin, IRHITextLayout* Layout, IRHIBrush* InBrush, ERHIDrawTextOptions Options = ERHIDrawTextOptions::None) override;

private:
	void AcquireWrappedTarget();
	void ReleaseWrappedTarget();
};