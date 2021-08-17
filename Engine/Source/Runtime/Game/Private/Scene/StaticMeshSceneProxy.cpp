// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Scene/StaticMeshSceneProxy.h"
#include "Scene/StaticMeshRenderData.h"
#include "Components/StaticMeshComponent.h"
#include "Assets/StaticMesh.h"

StaticMeshSceneProxy::StaticMeshSceneProxy(StaticMeshComponent* inComponent, const std::vector<MeshBatch>& batches) : Super(inComponent)
{
	MeshBatches = batches;
}