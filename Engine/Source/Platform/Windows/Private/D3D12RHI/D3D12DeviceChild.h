// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DXGIFactoryChild.h"
#include "DirectXCommon.h"
#include "RHI/IRHIDeviceChild.h"

class SD3D12Device;

class SD3D12DeviceChild : public SDXGIFactoryChild, implements IRHIDeviceChild
{
	GENERATED_BODY(SD3D12DeviceChild)

private:
	ID3D12DeviceChild* Object = nullptr;

protected:
	SD3D12Device* _device = nullptr;

public:
	SD3D12DeviceChild(SDXGIFactory* factory, SD3D12Device* device, ID3D12DeviceChild* InConstructorObject);

	virtual IRHIDevice* GetDevice() override;
	virtual void SetDebugName(std::wstring_view InName) override;
};