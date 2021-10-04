// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDevice;

class RENDERCORE_API SMaterialInterface : implements SObject
{
	GENERATED_BODY(SMaterialInterface)

public:
	SMaterialInterface(IRHIDevice* device);
};