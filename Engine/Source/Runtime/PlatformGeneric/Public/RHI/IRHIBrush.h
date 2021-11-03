// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/IRHIDeviceChild.h"

interface IRHIBrush : implements IRHIDeviceChild
{
	virtual void SetOpacity(float InValue) = 0;
	virtual float GetOpacity() = 0;
};