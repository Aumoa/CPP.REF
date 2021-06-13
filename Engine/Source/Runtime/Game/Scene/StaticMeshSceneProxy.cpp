// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Game;

StaticMeshSceneProxy::StaticMeshSceneProxy(StaticMeshComponent* inComponent) : Super(inComponent)
{
	StaticMesh* mesh = inComponent->GetStaticMesh();
	StaticMeshRenderData* rd = mesh->GetRenderData();

	MeshBatches = rd->MeshBatches;
}