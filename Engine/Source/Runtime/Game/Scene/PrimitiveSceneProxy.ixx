// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:PrimitiveSceneProxy;

import std.core;
import SC.Runtime.Core;
import :MeshBatch;

using namespace std;

export class PrimitiveSceneProxy : virtual public Object
{
public:
	using Super = Object;
	
public:
	PrimitiveSceneProxy();

protected:
	vector<MeshBatch> MeshBatches;
};