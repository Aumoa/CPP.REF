// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include <queue>

class World;
class RHIDevice;
class RHIDeviceContext;
class RHIShader;
class SceneVisibility;
class PrimitiveSceneProxy;
class Material;
class SceneRenderer;
struct MinimalViewInfo;

class GAME_API Scene : virtual public Object
{
	GENERATED_BODY(Scene)

	friend class SceneVisibility;
	friend class SceneRenderer;

private:
	static constexpr const size_t RenderQueue_Opaque = 1000;
	static constexpr const size_t RenderQueue_Masked = 2000;
	static constexpr const size_t RenderQueue_Transparent = 3000;

private:
	World* _world = nullptr;
	RHIDevice* _device = nullptr;
	std::vector<PrimitiveSceneProxy*> _primitives;
	std::queue<int64> _spaces;

	SceneVisibility* _localPlayerView = nullptr;
	std::map<size_t, std::vector<RHIShader*>> _renderQueue;
	std::vector<SceneRenderer> _renderers;

public:
	Scene(World* worldOwner, RHIDevice* device);
	~Scene() override;

	void UpdateScene(std::chrono::duration<float> elapsedTime);
	void InitViews(const MinimalViewInfo& localPlayerView);
	void RenderScene(RHIDeviceContext* dc);

	int64 AddPrimitive(PrimitiveSceneProxy* proxy);
	void RemovePrimitive(int64 primitiveId);

	RHIDevice* GetDevice() const { return _device; }

private:
	PrimitiveSceneProxy* RecreateRenderSceneProxy(PrimitiveSceneProxy* proxy);
};