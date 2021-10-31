// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIResource.h"
#include "RHIStructures.h"

interface IRHIBuffer : implements IRHIResource
{
	virtual RHIBufferDesc GetDesc() = 0;
};