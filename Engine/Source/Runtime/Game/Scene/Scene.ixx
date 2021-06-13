// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:Scene;

import SC.Runtime.Core;
import SC.Runtime.RenderCore;
import std.core;
import :MinimalViewInfo;

using namespace std::chrono;

export class World;
export class SceneVisibility;

export class Scene : virtual public Object
{
public:
	using Super = Object;

private:
	World* _world = nullptr;
	RHIDevice* _device = nullptr;
	SceneVisibility* _localPlayerView = nullptr;

public:
	Scene(World* worldOwner, RHIDevice* device);

	void InitViews(duration<float> elapsedTime, const MinimalViewInfo& localPlayerView);

	RHIDevice* GetDevice() const { return _device; }
};