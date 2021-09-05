// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "GameStructures.h"

class Scene;
class RHIShader;
class RHIDeviceContext;
class SceneVisibility;
class PrimitiveSceneProxy;

class GAME_API SceneRenderer : virtual public Object
{
	GENERATED_BODY(SceneRenderer)

private:
	struct MeshBatchDrawRelevance
	{
		size_t ViewIndex;
		PrimitiveSceneProxy* SceneProxy = nullptr;
		MeshBatch Batch;
		std::vector<MeshBatchElement> Elements;
		std::vector<Material*> Materials;
	};

private:
	Scene* _scene = nullptr;
	RHIShader* _shader = nullptr;
	SceneVisibility* _visibility = nullptr;
	std::vector<MeshBatchDrawRelevance> _drawRelevances;
	size_t _relevances = 0;

public:
	SceneRenderer(Scene* scene, RHIShader* shader);
	SceneRenderer(SceneRenderer&& rhs) noexcept;
	~SceneRenderer() override;

public:
	void CollectPrimitives(SceneVisibility* view);
	void RenderScene(RHIDeviceContext* dc);

private:
	void RenderWithSceneVisibility(RHIDeviceContext* dc);
};