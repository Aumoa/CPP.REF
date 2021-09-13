// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Scene/StaticMeshSceneProxy.h"
#include "Scene/StaticMeshRenderData.h"
#include "Components/StaticMeshComponent.h"
#include "Assets/StaticMesh.h"

SStaticMeshSceneProxy::SStaticMeshSceneProxy(SStaticMeshComponent* inComponent, const std::vector<MeshBatch>& batches) : Super(inComponent)
{
	MeshBatches = batches;
}