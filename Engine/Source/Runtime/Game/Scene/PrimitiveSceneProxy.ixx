// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:PrimitiveSceneProxy;

import std.core;
import SC.Runtime.Core;
import :MeshBatch;

using namespace std;

export class PrimitiveComponent;

export class PrimitiveSceneProxy : virtual public Object
{
public:
	using Super = Object;

private:
	PrimitiveComponent* _MyComponent = nullptr;
	
public:
	PrimitiveSceneProxy(PrimitiveComponent* inComponent);

	vector<MeshBatch> MeshBatches;
	int64 PrimitiveId = -1;
};