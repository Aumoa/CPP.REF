// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIDeviceChild.h"

interface IRHITextLayout : implements IRHIDeviceChild
{
	virtual Vector2 GetMaxSize() = 0;
};