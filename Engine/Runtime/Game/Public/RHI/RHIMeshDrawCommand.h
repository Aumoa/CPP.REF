// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

struct GAME_API RHIMeshDrawCommand
{
	uint64 VertexBufferVirtualAddress;
	uint64 IndexBufferVirtualAddress;

	uint32 VertexCount;
	uint32 IndexCount;
	uint32 VertexStride;
};