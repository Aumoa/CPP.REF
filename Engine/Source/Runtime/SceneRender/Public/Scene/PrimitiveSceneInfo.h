// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeshBatch.h"

struct PrimitiveSceneInfo
{
	int64 PrimitiveId;
	std::vector<MeshBatch> MeshBatches;
	Transform ComponentTransform;
	uint8 bHiddenInGame : 1;
};