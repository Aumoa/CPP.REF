// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:PrimitiveSceneProxy;

import std.core;
import SC.Runtime.Core;
import :MeshBatch;
import :Transform;

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

	PrimitiveComponent* GetComponent() const { return _MyComponent; }
	void UpdateTransform_GameThread(const Transform& value);

	vector<MeshBatch> MeshBatches;
	int64 PrimitiveId = -1;
	Transform ComponentTransform;
};