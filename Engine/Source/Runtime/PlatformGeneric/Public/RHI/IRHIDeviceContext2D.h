// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIDeviceChild.h"

interface IRHIDeviceContext2D : implements IRHIDeviceChild
{
	virtual void BeginDraw() = 0;
	virtual void EndDraw() = 0;
};