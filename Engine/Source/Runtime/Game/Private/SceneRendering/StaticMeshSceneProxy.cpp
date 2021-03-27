// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/StaticMeshSceneProxy.h"

#include "Components/StaticMeshComponent.h"
#include "SceneRendering/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXShaderResourceView.h"

StaticMeshSceneProxy::StaticMeshSceneProxy(GStaticMeshComponent* inMeshComponent) : Super(inMeshComponent)
	, meshComponent(inMeshComponent)
	, staticMesh(inMeshComponent->GetStaticMesh())
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
		return;
	}

	staticMesh = meshComponent->GetStaticMesh();

	if (staticMesh != nullptr)
	{
		InstanceShaderRecord.resize(0);
		DeferredShaderRecords.resize(0);
		Materials.resize(0);

		PrimitiveAccelerationPtr = staticMesh->RaytracingAccelerationStructureBuffer->GetGPUVirtualAddress();
		for (auto& subset : staticMesh->Subsets)
		{
			uint64 vertexBufferView = staticMesh->VertexBuffer->GetGPUVirtualAddress();
			vertexBufferView += subset.VertexStart * sizeof(Vertex);

			uint64 indexBufferView = staticMesh->IndexBuffer->GetGPUVirtualAddress();
			indexBufferView += subset.IndexStart * sizeof(uint32);

			DirectXInstanceShaderRecord& shaderRecord = InstanceShaderRecord.emplace_back();
			shaderRecord.ShaderIndex = 0;
			shaderRecord.RootParameters.emplace_back(vertexBufferView);
			shaderRecord.RootParameters.emplace_back(indexBufferView);
			shaderRecord.RootParameters.emplace_back(GetInstanceTransformBuf());

			DeferredShaderRecordApp& deferredApp = DeferredShaderRecords.emplace_back();

			if (subset.Material != nullptr)
			{
				deferredApp.ShaderRecordApps.emplace_back(subset.Material->SurfaceTextureSRV);
				Materials.emplace_back(subset.Material);
			}
		}
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