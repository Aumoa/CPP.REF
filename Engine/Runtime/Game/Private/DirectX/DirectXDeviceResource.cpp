// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXDeviceResource.h"

#include "DirectXMinimal.h"
#include "COM/COMMinimal.h"
#include "Logging/LogMacros.h"

DirectXDeviceResource::DirectXDeviceResource() : Super()
	, object(nullptr)
	, parent(nullptr)
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
	if (object == nullptr)
	{
		SE_LOG(LogDirectX, Error, TEXT("DeviceChild object is nullptr. For SetDebugName({0})"), value);
		return;
	}

	debugName = value;
	HR(object->SetName(debugName->C_Str));
}

void DirectXDeviceResource::SetDeviceChildPtr(ID3D12DeviceChild* ptr, DirectXDeviceBundle* parent)
{
	object = ptr;
	this->parent = parent;
}