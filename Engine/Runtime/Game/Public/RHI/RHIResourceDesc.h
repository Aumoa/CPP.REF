// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

enum class ERHITextureFormat;

struct GAME_API RHIResourceDesc
{
	ERHITextureFormat Format;
	uint64 Width;
	uint32 Height;
	uint16 DepthOrArraySize;

	RHIResourceDesc();
	~RHIResourceDesc();
};