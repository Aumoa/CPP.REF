// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12DeviceChild.h"
#include "D3D12Device.h"

GENERATE_BODY(SD3D12DeviceChild);

SD3D12DeviceChild::SD3D12DeviceChild(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID3D12DeviceChild* InConstructorObject) : Super(InFactory)
	, Object(InConstructorObject)
	, Device(InDevice)
{
}

IRHIDevice* SD3D12DeviceChild::GetDevice()
{
	return Device;
}

void SD3D12DeviceChild::SetDebugName(std::wstring_view InName)
{
	if (Object)
	{
		HR(Object->SetName(InName.data()));
	}
}