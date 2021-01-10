// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/PrimitiveSceneProxy.h"

PrimitiveSceneProxy::PrimitiveSceneProxy(PrimitiveComponent* inPrimitiveComponent) : Super()
	, myPrimitiveComponent(inPrimitiveComponent)
{

}

PrimitiveSceneProxy::~PrimitiveSceneProxy()
{

}

void PrimitiveSceneProxy::UpdateTransform(const Transform& value)
{
	transform = value;
}

MeshBatch* PrimitiveSceneProxy::GetMeshBatch() const
{
	return nullptr;
}

Transform PrimitiveSceneProxy::GetPrimitiveTransform() const
{
	return transform;
}