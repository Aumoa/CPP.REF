// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "COM/TComPtr.h"

interface IDXGIAdapter1;
interface IDXGIFactory2;
interface ID3D12Device5;

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

	void Initialize();

private:
	bool IsAdapterSuitable(IDXGIAdapter1* adapter) const;
	bool IsDeviceSuitable(ID3D12Device5* device) const;
};