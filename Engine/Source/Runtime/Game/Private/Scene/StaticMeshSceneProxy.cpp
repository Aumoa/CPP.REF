// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Scene/StaticMeshSceneProxy.h"
#include "Components/StaticMeshComponent.h"

StaticMeshSceneProxy::StaticMeshSceneProxy(SStaticMeshComponent* InStaticMeshComponent, const std::vector<MeshBatch>& InBatches)
	: PrimitiveSceneProxy(InStaticMeshComponent)
{
	MeshBatches = InBatches;
}