// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "RHI/IRHIBrush.h"

class DIRECTX_API SD2D1Brush : public SD3D12DeviceChild, implements IRHIBrush
{
	GENERATED_BODY(SD2D1Brush)

private:
	ComPtr<ID2D1Brush> BrushObject;

public:
	SD2D1Brush(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID2D1Brush* InBrush);

	virtual void SetOpacity(float InValue) override;
	virtual float GetOpacity() override;

protected:
	virtual void Dispose(bool bDisposing) override;

public:
	DECLARE_GETTER(ID2D1Brush, BrushObject);
};