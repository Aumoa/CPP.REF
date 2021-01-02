// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/StaticMeshSceneProxy.h"

#include "Components/StaticMeshComponent.h"
#include "SceneRendering/StaticMesh.h"

StaticMeshSceneProxy::StaticMeshSceneProxy(StaticMeshComponent* inMeshComponent) : Super(inMeshComponent)
	, staticMesh(nullptr)
{
	staticMesh = inMeshComponent->GetStaticMesh();
}

StaticMeshSceneProxy::~StaticMeshSceneProxy()
{

}

MeshBatch* StaticMeshSceneProxy::GetMeshBatch() const
{
	return staticMesh->GetMeshBatch();
}