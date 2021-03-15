// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "TComPtr.h"

interface IWICImagingFactory;

class GAME_API COMDeviceBundle : virtual public Object
{
public:
	using Super = Object;

private:
	TComPtr<IWICImagingFactory> imagingFactory;

public:
	COMDeviceBundle();
	~COMDeviceBundle() override;

	IWICImagingFactory* GetImagingFactory() const;

private:
	void Initialize();
};