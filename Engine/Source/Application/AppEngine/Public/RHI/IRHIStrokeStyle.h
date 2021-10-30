// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIStructures.h"

interface IRHIStrokeStyle : implements SObject
{
	virtual RHIStrokeStyleDesc GetDesc() = 0;
	virtual std::vector<float> GetDashes() = 0;
};