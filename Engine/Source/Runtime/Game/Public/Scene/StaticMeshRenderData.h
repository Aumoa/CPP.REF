// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "GameStructures.h"

class SMaterial;
class SRHIVertexFactory;
struct MeshBatch;

class GAME_API SStaticMeshRenderData : virtual public SObject
{
	GENERATED_BODY(SStaticMeshRenderData)

public:
	SStaticMeshRenderData();

	std::vector<MeshBatch> MeshBatches;
};