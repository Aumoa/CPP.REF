// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include <queue>

interface IRHIDevice;
interface IRHIDeviceContext;
interface IRHIShader;
class SWorld;
class SSceneVisibility;
class SPrimitiveSceneProxy;
class SMaterial;
class SSceneRenderer;
struct MinimalViewInfo;

class GAME_API SScene : implements SObject
{
	GENERATED_BODY(SScene)

	friend class SSceneVisibility;
	friend class SSceneRenderer;

private:
	static constexpr const size_t RenderQueue_Opaque = 1000;
	static constexpr const size_t RenderQueue_Masked = 2000;
	static constexpr const size_t RenderQueue_Transparent = 3000;

private:
	SWorld* _world = nullptr;
	IRHIDevice* _device = nullptr;
	std::vector<SPrimitiveSceneProxy*> _primitives;
	std::queue<int64> _spaces;

	SSceneVisibility* _localPlayerView = nullptr;
	std::map<size_t, std::vector<IRHIShader*>> _renderQueue;
	std::vector<SSceneRenderer> _renderers;

public:
	SScene(SWorld* worldOwner, IRHIDevice* device);
	~SScene() override;

	void UpdateScene(std::chrono::duration<float> elapsedTime);
	void InitViews(const MinimalViewInfo& localPlayerView);
	void RenderScene(IRHIDeviceContext* dc);

	int64 AddPrimitive(SPrimitiveSceneProxy* proxy);
	void RemovePrimitive(int64 primitiveId);

	IRHIDevice* GetDevice() const { return _device; }

private:
	SPrimitiveSceneProxy* RecreateRenderSceneProxy(SPrimitiveSceneProxy* proxy);
};