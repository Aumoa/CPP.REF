// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIFactoryChild.h"

interface IRHIDevice;

interface IRHIDeviceChild : implements IRHIFactoryChild
{
	GENERATED_INTERFACE_BODY(IRHIDeviceChild)

	virtual IRHIDevice* GetDevice() = 0;
	virtual void SetDebugName(std::wstring_view InName) = 0;
};