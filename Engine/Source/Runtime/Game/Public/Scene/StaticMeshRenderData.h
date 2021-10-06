// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameStructures.h"

class SMaterial;
struct MeshBatch;

class GAME_API SStaticMeshRenderData : implements SObject
{
	GENERATED_BODY(SStaticMeshRenderData)

public:
	SStaticMeshRenderData();

	std::vector<MeshBatch> MeshBatches;
};