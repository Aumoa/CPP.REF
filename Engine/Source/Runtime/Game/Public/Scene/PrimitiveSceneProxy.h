// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "RHI/RHIStructures.h"
#include "SceneRendering/MeshBatch.h"
#include "Threading/Task.h"

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

	Task<void> UpdateTransform_GameThread(Transform InValue);
	void UpdateTransform_RenderThread(const Transform& InValue);
	Task<void> MarkRenderStateDirty_GameThread();
	void MarkRenderStateDirty_RenderThread();
	Task<void> SetHiddenInGame_GameThread(bool bHiddenInGame);
	void SetHiddenInGame_RenderThread(bool bHiddenInGame);
};