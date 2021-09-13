// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "GameStructures.h"

class SScene;
class SRHIShader;
class SRHIDeviceContext;
class SSceneVisibility;
class SPrimitiveSceneProxy;

class GAME_API SSceneRenderer : virtual public SObject
{
	GENERATED_BODY(SSceneRenderer)

private:
	struct MeshBatchDrawRelevance
	{
		size_t ViewIndex;
		SPrimitiveSceneProxy* SceneProxy = nullptr;
		MeshBatch Batch;
		std::vector<MeshBatchElement> Elements;
		std::vector<SMaterial*> Materials;
	};

private:
	SScene* _scene = nullptr;
	SRHIShader* _shader = nullptr;
	SSceneVisibility* _visibility = nullptr;
	std::vector<MeshBatchDrawRelevance> _drawRelevances;
	size_t _relevances = 0;

public:
	SSceneRenderer(SScene* scene, SRHIShader* shader);
	SSceneRenderer(SSceneRenderer&& rhs) noexcept;
	~SSceneRenderer() override;

public:
	void CollectPrimitives(SSceneVisibility* view);
	void RenderScene(SRHIDeviceContext* dc);

private:
	void RenderWithSceneVisibility(SRHIDeviceContext* dc);
};