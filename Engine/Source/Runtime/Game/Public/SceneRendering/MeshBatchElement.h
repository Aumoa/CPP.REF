// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct MeshBatchElement
{
	uint32 IndexCount;
	uint32 InstanceCount;
	int32 StartIndexLocation;
	int32 BaseVertexLocation;
	uint32 VertexCount;
	uint32 MaterialSlotIndex;
};