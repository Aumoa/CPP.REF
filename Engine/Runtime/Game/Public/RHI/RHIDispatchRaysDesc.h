// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

struct GAME_API RHIDispatchRaysDesc
{
	uint32 Width;
	uint32 Height;

	RHIDispatchRaysDesc();
};