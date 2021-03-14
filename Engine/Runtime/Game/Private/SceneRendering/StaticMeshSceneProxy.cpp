// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/StaticMeshSceneProxy.h"

#include "Components/StaticMeshComponent.h"
#include "SceneRendering/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "RHI/IRHIResource.h"
#include "DirectX/DirectXCommon.h"

StaticMeshSceneProxy::StaticMeshSceneProxy(GStaticMeshComponent* inMeshComponent) : Super(inMeshComponent)
	, meshComponent(inMeshComponent)
	, staticMesh(inMeshComponent->GetStaticMesh())
	, material(nullptr)
{
	Update();
}

StaticMeshSceneProxy::~StaticMeshSceneProxy()
{

}

void StaticMeshSceneProxy::Update()
{
	Super::Update();

	if (!meshComponent.IsValid)
	{
		staticMesh = nullptr;
		material = nullptr;
		return;
	}

	staticMesh = meshComponent->GetStaticMesh();
	material = meshComponent->GetMaterial();

	if (staticMesh != nullptr)
	{
		PrimitiveAccelerationPtr = staticMesh->RaytracingAccelerationStructureBuffer->GetGPUVirtualAddress();
		InstanceShaderRecord.ShaderIndex = 0;
		InstanceShaderRecord.RootParameters.resize(0);
		InstanceShaderRecord.RootParameters.emplace_back(staticMesh->VertexBuffer->GetGPUVirtualAddress());
		InstanceShaderRecord.RootParameters.emplace_back(staticMesh->IndexBuffer->GetGPUVirtualAddress());
		InstanceShaderRecord.RootParameters.emplace_back(GetInstanceTransformBuf());
	}
}

void StaticMeshSceneProxy::UpdateTransform()
{
	Super::UpdateTransform();

	if (staticMesh == nullptr)
	{
		return;
	}

	auto trp = PrimitiveTransform;
	AxisAlignedCube baseBoundingBox = staticMesh->BoundingBox;

	Vector3 bp = baseBoundingBox.GetPoint(0);
	bp = trp.TransformVector(bp);
	PrimitiveBoundingBox.Min = bp;
	PrimitiveBoundingBox.Max = bp;

	for (size_t i = 1; i < 8; ++i)
	{
		bp = baseBoundingBox.GetPoint(i);
		bp = trp.TransformVector(bp);
		PrimitiveBoundingBox.Min = Vector3::Min(PrimitiveBoundingBox.Min, bp);
		PrimitiveBoundingBox.Max = Vector3::Max(PrimitiveBoundingBox.Max, bp);
	}
}