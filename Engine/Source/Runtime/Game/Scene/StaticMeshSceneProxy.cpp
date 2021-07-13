// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "StaticMeshSceneProxy.h"
#include "StaticMeshRenderData.h"
#include "Components/StaticMeshComponent.h"
#include "Assets/StaticMesh.h"

using namespace std;

StaticMeshSceneProxy::StaticMeshSceneProxy(StaticMeshComponent* inComponent, const vector<MeshBatch>& batches) : Super(inComponent)
{
	MeshBatches = batches;
}