// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Scene/StaticMeshSceneProxy.h"
#include "Components/StaticMeshComponent.h"

StaticMeshSceneProxy::StaticMeshSceneProxy(SStaticMeshComponent* InStaticMeshComponent, std::span<MeshBatch> InBatches)
	: PrimitiveSceneProxy(InStaticMeshComponent)
{
	MeshBatches = InBatches;
}