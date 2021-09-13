// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PrimitiveSceneProxy.h"

class SStaticMeshComponent;

class GAME_API SStaticMeshSceneProxy : public SPrimitiveSceneProxy
{
	GENERATED_BODY(SStaticMeshSceneProxy)

public:
	SStaticMeshSceneProxy(SStaticMeshComponent* inComponent, const std::vector<MeshBatch>& batches);
};