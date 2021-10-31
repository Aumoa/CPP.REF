// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIDeviceChild.h"

interface IRHIResource : implements IRHIDeviceChild
{
	virtual uint64 GetGPUVirtualAddress() = 0;
};