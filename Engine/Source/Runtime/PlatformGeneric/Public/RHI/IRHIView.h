// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIDeviceChild.h"

interface IRHIResource;

interface IRHIView : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIView)

	virtual int32 GetViewCount() = 0;
	virtual IRHIResource* GetResource(int32 indexOf) = 0;
};