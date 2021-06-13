// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Game;
import std.core;

using namespace std::chrono;

Scene::Scene(World* worldOwner, RHIDevice* device) : Super()
	, _world(worldOwner)
	, _device(device)
{
	_localPlayerView = CreateSubobject<SceneVisibility>(this);
}

void Scene::InitViews(duration<float> elapsedTime, const MinimalViewInfo& localPlayerView)
{
	_localPlayerView->CalcVisibility(elapsedTime, localPlayerView);
}