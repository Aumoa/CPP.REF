// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/DXGI/DXGIFactoryChild.h"
#include "RHI/IRHIDeviceChild.h"

class SD3D12Device;

class SD3D12DeviceChild : public SDXGIFactoryChild, implements IRHIDeviceChild
{
	GENERATED_BODY(SD3D12DeviceChild)

protected:
	SD3D12Device* _device = nullptr;

public:
	SD3D12DeviceChild(SDXGIFactory* factory, SD3D12Device* device);

	virtual IRHIDevice* GetDevice() override;
};