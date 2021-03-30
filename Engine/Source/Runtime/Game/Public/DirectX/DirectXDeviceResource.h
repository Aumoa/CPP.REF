// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "DirectXMinimal.h"

class DirectXDeviceBundle;

class GAME_API DirectXDeviceResource : virtual public Object
{
public:
	using Super = Object;

private:
	ID3D12DeviceChild* object;
	DirectXDeviceBundle* parent;
	TRefPtr<String> debugName;

public:
	DirectXDeviceResource(DirectXDeviceBundle* deviceBundle);
	~DirectXDeviceResource() override;

	vs_property_virtual(TRefPtr<String>, DebugName);
	DirectXDeviceBundle* GetDevice() const;

protected:
	void SetDeviceChildPtr(ID3D12DeviceChild* ptr);
};