// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PrimitiveSceneProxy.h"

class SStaticMeshComponent;

class GAME_API StaticMeshSceneProxy : public PrimitiveSceneProxy
{
public:
	StaticMeshSceneProxy(SStaticMeshComponent* InStaticMeshComponent, std::span<MeshBatch> InBatches);
};