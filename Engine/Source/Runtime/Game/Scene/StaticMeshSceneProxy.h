// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PrimitiveSceneProxy.h"

class StaticMeshComponent;

class StaticMeshSceneProxy : public PrimitiveSceneProxy
{
public:
	using Super = PrimitiveSceneProxy;

public:
	StaticMeshSceneProxy(StaticMeshComponent* inComponent, const std::vector<MeshBatch>& batches);
};