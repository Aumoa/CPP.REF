// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "GameStructures.h"

class Material;
class RHIVertexFactory;
struct MeshBatch;

class GAME_API StaticMeshRenderData : virtual public Object
{
	GENERATED_BODY(StaticMeshRenderData)

public:
	StaticMeshRenderData();

	std::vector<MeshBatch> MeshBatches;
};