// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"

class RHIVertexFactory;
struct MeshBatch;

class StaticMeshRenderData : virtual public Object
{
public:
	using Super = Object;

public:
	StaticMeshRenderData();

	std::vector<MeshBatch> MeshBatches;
};