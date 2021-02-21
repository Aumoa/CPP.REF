// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIShaderBindingTable;

struct GAME_API RHIDispatchRaysDesc
{
	IRHIShaderBindingTable* SBT;
	uint32 Width;
	uint32 Height;

	RHIDispatchRaysDesc();
};