// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "DirectXMinimal.h"

class GAME_API DirectXDeviceBundle : virtual public Object
{
public:
	using Super = Object;

private:
	TComPtr<IDXGIFactory2> dxgiFactory;
	TComPtr<ID3D12Device5> device;

public:
	DirectXDeviceBundle();
	~DirectXDeviceBundle() override;

	IDXGIFactory2* GetFactory() const;
	ID3D12Device5* GetDevice() const;

private:
	bool IsAdapterSuitable(IDXGIAdapter1* adapter) const;
	bool IsDeviceSuitable(ID3D12Device5* device) const;
};