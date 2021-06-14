// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:Scene;

import std.core;
import SC.Runtime.Core;
import SC.Runtime.RenderCore;
import :MinimalViewInfo;

using namespace std;
using namespace std::chrono;

export class World;
export class SceneVisibility;
export class PrimitiveSceneProxy;

export class Scene : virtual public Object
{
	friend class SceneVisibility;
	friend class SceneRenderer;

public:
	using Super = Object;

private:
	World* _world = nullptr;
	RHIDevice* _device = nullptr;
	SceneVisibility* _localPlayerView = nullptr;
	vector<PrimitiveSceneProxy*> _primitives;
	queue<int64> _spaces;

public:
	Scene(World* worldOwner, RHIDevice* device);

	void UpdateScene(duration<float> elapsedTime);
	void InitViews(const MinimalViewInfo& localPlayerView);

	int64 AddPrimitive(PrimitiveSceneProxy* proxy);
	void RemovePrimitive(int64 primitiveId);

	RHIDevice* GetDevice() const { return _device; }
};