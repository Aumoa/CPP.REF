// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "COM/TComPtr.h"

interface ID3D12DeviceChild;
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
	DirectXDeviceResource();
	~DirectXDeviceResource() override;

	vs_property(TRefPtr<String>, DebugName);
	DirectXDeviceBundle* GetDevice() const;

protected:
	void SetDeviceChildPtr(ID3D12DeviceChild* ptr, DirectXDeviceBundle* parent);
};