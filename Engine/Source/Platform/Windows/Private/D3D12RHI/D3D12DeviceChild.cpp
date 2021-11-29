// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12DeviceChild.h"
#include "D3D12Device.h"

SD3D12DeviceChild::SD3D12DeviceChild(SDXGIFactory* factory, SD3D12Device* device, ID3D12DeviceChild* InConstructorObject) : Super(factory)
	, Object(InConstructorObject)
	, _device(device)
{
}

IRHIDevice* SD3D12DeviceChild::GetDevice()
{
	return _device;
}

void SD3D12DeviceChild::SetDebugName(std::wstring_view InName)
{
	if (Object)
	{
		HR(Object->SetName(InName.data()));
	}
}