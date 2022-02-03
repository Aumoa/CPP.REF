// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneRendering/MeshBatch.h"

class SMaterial;

class GAME_API SStaticMeshRenderData : implements SObject
{
	GENERATED_BODY(SStaticMeshRenderData)

public:
	SStaticMeshRenderData();

	std::vector<MeshBatch> MeshBatches;
};