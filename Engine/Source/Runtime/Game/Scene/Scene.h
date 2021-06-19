// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include <queue>

class World;
class RHIDevice;
class SceneVisibility;
class PrimitiveSceneProxy;
struct MinimalViewInfo;

class Scene : virtual public Object
{
	friend class SceneVisibility;
	friend class SceneRenderer;

public:
	using Super = Object;

private:
	World* _world = nullptr;
	RHIDevice* _device = nullptr;
	SceneVisibility* _localPlayerView = nullptr;
	std::vector<PrimitiveSceneProxy*> _primitives;
	std::queue<int64> _spaces;

public:
	Scene(World* worldOwner, RHIDevice* device);

	void UpdateScene(std::chrono::duration<float> elapsedTime);
	void InitViews(const MinimalViewInfo& localPlayerView);

	int64 AddPrimitive(PrimitiveSceneProxy* proxy);
	void RemovePrimitive(int64 primitiveId);

	RHIDevice* GetDevice() const { return _device; }
};