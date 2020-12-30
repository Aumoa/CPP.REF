// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/PrimitiveSceneProxy.h"

PrimitiveSceneProxy::PrimitiveSceneProxy(PrimitiveComponent* inPrimitiveComponent) : Super()
	, myPrimitiveComponent(inPrimitiveComponent)
{

}

PrimitiveSceneProxy::~PrimitiveSceneProxy()
{

}

MeshBatch* PrimitiveSceneProxy::GetMeshBatch() const
{
	return nullptr;
}