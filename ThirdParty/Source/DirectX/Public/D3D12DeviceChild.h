// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DXGIFactoryChild.h"
#include "DirectXCommon.h"
#include "RHI/IRHIDeviceChild.h"

class SD3D12Device;

class DIRECTX_API SD3D12DeviceChild : public SDXGIFactoryChild, implements IRHIDeviceChild
{
	GENERATED_BODY(SD3D12DeviceChild)

private:
	ID3D12DeviceChild* Object = nullptr;

protected:
	SPROPERTY(Device)
	SD3D12Device* Device = nullptr;

public:
	SD3D12DeviceChild(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID3D12DeviceChild* InConstructorObject);

	using Super::Dispose;
	virtual IRHIDevice* GetDevice() override;
	virtual void SetDebugName(std::wstring_view InName) override;

protected:
	virtual void Dispose(bool bDisposing) override;
};