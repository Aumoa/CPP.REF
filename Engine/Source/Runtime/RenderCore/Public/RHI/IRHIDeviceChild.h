// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIFactoryChild.h"

interface IRHIDevice;

interface IRHIDeviceChild : implements IRHIFactoryChild
{
	virtual IRHIDevice* GetDevice() = 0;
};