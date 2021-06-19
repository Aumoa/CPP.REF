// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "StaticMeshSceneProxy.h"
#include "StaticMeshRenderData.h"
#include "Components/StaticMeshComponent.h"
#include "Assets/StaticMesh.h"

StaticMeshSceneProxy::StaticMeshSceneProxy(StaticMeshComponent* inComponent) : Super(inComponent)
{
	StaticMesh* mesh = inComponent->GetStaticMesh();
	StaticMeshRenderData* rd = mesh->GetRenderData();

	MeshBatches = rd->MeshBatches;
}