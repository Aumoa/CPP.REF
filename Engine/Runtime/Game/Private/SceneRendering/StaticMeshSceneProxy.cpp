// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/StaticMeshSceneProxy.h"

#include "Components/StaticMeshComponent.h"
#include "SceneRendering/StaticMesh.h"

StaticMeshSceneProxy::StaticMeshSceneProxy(StaticMeshComponent* inMeshComponent) : Super(inMeshComponent)
	, staticMesh(nullptr)
{
	staticMesh = inMeshComponent->GetStaticMesh();
	baseBoundingBox = staticMesh->BoundingBox;
}

StaticMeshSceneProxy::~StaticMeshSceneProxy()
{

}

void StaticMeshSceneProxy::UpdateMovable()
{
	auto trp = GetPrimitiveTransform();

	Vector3 bp = baseBoundingBox.GetPoint(0);
	bp = trp.TransformVector(bp);
	transformedBoundingBox.Min = bp;
	transformedBoundingBox.Max = bp;

	for (size_t i = 1; i < 8; ++i)
	{
		bp = baseBoundingBox.GetPoint(i);
		bp = trp.TransformVector(bp);
		transformedBoundingBox.Min = Vector3::Min(transformedBoundingBox.Min, bp);
		transformedBoundingBox.Max = Vector3::Max(transformedBoundingBox.Max, bp);
	}
}

MeshBatch* StaticMeshSceneProxy::GetMeshBatch() const
{
	return staticMesh->GetMeshBatch();
}

const AxisAlignedCube* StaticMeshSceneProxy::GetPrimitiveBoundingBox() const
{
	return &transformedBoundingBox;
}