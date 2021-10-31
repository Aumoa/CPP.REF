// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIResource.h"
#include "RHIStructures.h"

interface IRHITexture2D : implements IRHIResource
{
	virtual RHITexture2DDesc GetDesc() = 0;
};