// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include <queue>

class World;
class SRHIDevice;
class SRHIDeviceContext;
class SRHIShader;
class SSceneVisibility;
class SPrimitiveSceneProxy;
class SMaterial;
class SSceneRenderer;
struct MinimalViewInfo;

class GAME_API SScene : virtual public SObject
{
	GENERATED_BODY(SScene)

	friend class SSceneVisibility;
	friend class SSceneRenderer;

private:
	static constexpr const size_t RenderQueue_Opaque = 1000;
	static constexpr const size_t RenderQueue_Masked = 2000;
	static constexpr const size_t RenderQueue_Transparent = 3000;

private:
	World* _world = nullptr;
	SRHIDevice* _device = nullptr;
	std::vector<SPrimitiveSceneProxy*> _primitives;
	std::queue<int64> _spaces;

	SSceneVisibility* _localPlayerView = nullptr;
	std::map<size_t, std::vector<SRHIShader*>> _renderQueue;
	std::vector<SSceneRenderer> _renderers;

public:
	SScene(World* worldOwner, SRHIDevice* device);
	~SScene() override;

	void UpdateScene(std::chrono::duration<float> elapsedTime);
	void InitViews(const MinimalViewInfo& localPlayerView);
	void RenderScene(SRHIDeviceContext* dc);

	int64 AddPrimitive(SPrimitiveSceneProxy* proxy);
	void RemovePrimitive(int64 primitiveId);

	SRHIDevice* GetDevice() const { return _device; }

private:
	SPrimitiveSceneProxy* RecreateRenderSceneProxy(SPrimitiveSceneProxy* proxy);
};