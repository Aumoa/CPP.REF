// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/StaticMeshSceneProxy.h"

#include "Components/StaticMeshComponent.h"
#include "SceneRendering/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "RHI/IRHIResource.h"

class StaticMeshSceneProxyBatch : public MeshBatch
{
public:
	using Super = MeshBatch;

private:
	RHIMeshDrawCommand drawCommand;

public:
	StaticMeshSceneProxyBatch(const RHIMeshDrawCommand& inDrawCommand, uint16 materialIndexOverride)
	{
		drawCommand = inDrawCommand;
		drawCommand.MaterialIndex = materialIndexOverride;
	}

	~StaticMeshSceneProxyBatch()
	{

	}

	const RHIMeshDrawCommand* GetDrawCommand() const override
	{
		return &drawCommand;
	}
};

StaticMeshSceneProxy::StaticMeshSceneProxy(GStaticMeshComponent* inMeshComponent) : Super(inMeshComponent)
	, meshComponent(inMeshComponent)
	, staticMesh(inMeshComponent->GetStaticMesh())
	, materialIndex(0)
{
	if (staticMesh != nullptr)
	{
		baseBoundingBox = staticMesh->BoundingBox;
		materialIndex = meshComponent->GetMaterial()->Index;
	}
}

StaticMeshSceneProxy::~StaticMeshSceneProxy()
{

}

void StaticMeshSceneProxy::Update()
{
	if (!meshComponent.IsValid)
	{
		staticMesh = nullptr;
		materialIndex = 0;
		return;
	}

	staticMesh = meshComponent->GetStaticMesh();
	materialIndex = meshComponent->GetMaterial()->Index;

	customBatch.Reset();
	if (staticMesh != nullptr)
	{
		const RHIMeshDrawCommand* drawCommand = staticMesh->GetMeshBatch()->GetDrawCommand();
		if (drawCommand != nullptr)
		{
			customBatch = NewObject<StaticMeshSceneProxyBatch>(*drawCommand, materialIndex);
		}
	}
}

void StaticMeshSceneProxy::UpdateTransform()
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
	return customBatch.Get();
}

const AxisAlignedCube* StaticMeshSceneProxy::GetPrimitiveBoundingBox() const
{
	return &transformedBoundingBox;
}

uint64 StaticMeshSceneProxy::GetRaytracingAccelerationStructurePtr() const
{
	return staticMesh->RaytracingAccelerationStructureBuffer->GetVirtualAddress();
}