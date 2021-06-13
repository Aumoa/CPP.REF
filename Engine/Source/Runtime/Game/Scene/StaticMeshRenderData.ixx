// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:StaticMeshRenderData;

import std.core;
import SC.Runtime.Core;
import SC.Runtime.RenderCore;
import :MeshBatch;

using namespace std;

export class StaticMeshRenderData : virtual public Object
{
public:
	using Super = Object;

public:
	StaticMeshRenderData(RHIVertexFactory* vfactory);

	vector<MeshBatch> MeshBatches;
};