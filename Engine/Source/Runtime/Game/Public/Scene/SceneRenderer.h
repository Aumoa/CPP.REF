// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameStructures.h"

interface IRHIShader;
interface IRHIDeviceContext;
class SScene;
class SSceneVisibility;
class SPrimitiveSceneProxy;

class GAME_API SSceneRenderer : implements SObject
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
	IRHIShader* _shader = nullptr;
	SSceneVisibility* _visibility = nullptr;
	std::vector<MeshBatchDrawRelevance> _drawRelevances;
	size_t _relevances = 0;

public:
	SSceneRenderer(SScene* scene, IRHIShader* shader);
	SSceneRenderer(SSceneRenderer&& rhs) noexcept;
	~SSceneRenderer() override;

public:
	void CollectPrimitives(SSceneVisibility* view);
	void RenderScene(IRHIDeviceContext* dc);

private:
	void RenderWithSceneVisibility(IRHIDeviceContext* dc);
};