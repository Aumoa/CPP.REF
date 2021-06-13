// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Game;

StaticMeshComponent::StaticMeshComponent() : Super()
{
}

PrimitiveSceneProxy* StaticMeshComponent::CreateSceneProxy()
{
	return new StaticMeshSceneProxy(this);
}