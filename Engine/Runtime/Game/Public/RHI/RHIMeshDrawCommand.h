// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

enum class ERHIPrimitiveTopology
{
	TriangleList = 4,
	TriangleStrip = 5
};

struct GAME_API RHIMeshDrawCommand
{
	ERHIPrimitiveTopology Topology;

	uint64 VertexBufferVirtualAddress;
	uint64 IndexBufferVirtualAddress;

	uint32 VertexCount;
	uint32 IndexCount;
	uint32 VertexStride;

	RHIMeshDrawCommand();
};