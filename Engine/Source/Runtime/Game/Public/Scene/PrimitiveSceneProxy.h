// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "RHI/RHIStructures.h"
#include "SceneRendering/MeshBatch.h"

class SPrimitiveComponent;

class GAME_API PrimitiveSceneProxy
{
public:
	SPrimitiveComponent* const PrimitiveComponent;
	int64 PrimitiveId;
	std::span<MeshBatch> MeshBatches;
	Transform ComponentTransform;
	uint8 bRenderStateDirty : 1;
	uint8 bHiddenInGame : 1;

	PrimitiveSceneProxy(SPrimitiveComponent* InPrimitiveComponent);

	void UpdateTransform_GameThread(const Transform& InValue);
	void UpdateTransform_RenderThread(const Transform& InValue);
	void MarkRenderStateDirty_GameThread();
	void MarkRenderStateDirty_RenderThread();
	void SetHiddenInGame_GameThread(bool bHiddenInGame);
	void SetHiddenInGame_RenderThread(bool bHiddenInGame);
};