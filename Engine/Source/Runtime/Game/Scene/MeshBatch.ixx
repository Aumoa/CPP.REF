// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:MeshBatch;

import std.core;
import :MeshBatchElement;

using namespace std;

export struct MeshBatch
{
	vector<MeshBatchElement> Elements;
	RHIVertexFactory* VertexFactory = nullptr;
};