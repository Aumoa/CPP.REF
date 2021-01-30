// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/PrimitiveSceneProxy.h"

#include "Components/PrimitiveComponent.h"

PrimitiveSceneProxy::PrimitiveSceneProxy(PrimitiveComponent* inPrimitiveComponent) : Super()
	, myPrimitiveComponent(inPrimitiveComponent)
	, Mobility(inPrimitiveComponent->Mobility)
{
	if (Mobility == EComponentMobility::Static)
	{
		transform = inPrimitiveComponent->ComponentTransform;
	}
}

PrimitiveSceneProxy::~PrimitiveSceneProxy()
{

}

void PrimitiveSceneProxy::UpdateMovable()
{
	transform = myPrimitiveComponent->ComponentTransform;
}

MeshBatch* PrimitiveSceneProxy::GetMeshBatch() const
{
	return nullptr;
}

Transform PrimitiveSceneProxy::GetPrimitiveTransform() const
{
	return transform;
}