// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXDeviceResource.h"

#include "DirectXCommon.h"
#include "Logging/LogMacros.h"

DirectXDeviceResource::DirectXDeviceResource(DirectXDeviceBundle* deviceBundle) : Super()
	, object(nullptr)
	, parent(deviceBundle)
{

}

DirectXDeviceResource::~DirectXDeviceResource()
{

}

TRefPtr<String> DirectXDeviceResource::DebugName_get() const
{
	return debugName;
}

void DirectXDeviceResource::DebugName_set(TRefPtr<String> value)
{
	debugName = value;

	if (object != nullptr)
	{
		HR(object->SetName(debugName->C_Str));
	}
}

DirectXDeviceBundle* DirectXDeviceResource::GetDevice() const
{
	return parent;
}

void DirectXDeviceResource::SetDeviceChildPtr(ID3D12DeviceChild* ptr)
{
	object = ptr;

	if (object != nullptr && debugName.IsValid)
	{
		object->SetName(debugName->C_Str);
	}
}