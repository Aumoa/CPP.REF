// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "RHI/IRHIDeviceContext2D.h"

class SD2D1DeviceContext : public SD3D12DeviceChild, implements IRHIDeviceContext2D
{
	GENERATED_BODY(SD2D1DeviceContext)

private:
	ComPtr<ID2D1DeviceContext> DeviceContext;

public:
	SD2D1DeviceContext(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID2D1DeviceContext* InDeviceContext);

	virtual void BeginDraw() override;
	virtual void EndDraw() override;
};