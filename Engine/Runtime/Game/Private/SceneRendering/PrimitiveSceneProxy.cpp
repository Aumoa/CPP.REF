// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/PrimitiveSceneProxy.h"

#include "Engine.h"
#include "Components/PrimitiveComponent.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "Logging/LogMacros.h"
#include "GameplayStatics/ClassDependencyHelper.h"
#include "Shaders/ShaderTypes.h"

PrimitiveSceneProxy::PrimitiveSceneProxy(GPrimitiveComponent* inPrimitiveComponent) : Super()
	, myPrimitiveComponent(inPrimitiveComponent)

	, Mobility(inPrimitiveComponent->Mobility)
	, PrimitiveId(0)
	, PrimitiveBoundingBox{ }
	, PrimitiveAccelerationPtr(0)
{
	Engine* engine = ClassDependencyHelper::GetEngine(inPrimitiveComponent);
	DirectXDeviceBundle* deviceBundle = engine->GetDeviceBundle();

	if (Mobility == EComponentMobility::Static)
	{
		PrimitiveTransform = inPrimitiveComponent->ComponentTransform;

		RaytracingInstanceTransform trpInstanced;
		trpInstanced.World = PrimitiveTransform.Matrix;
		trpInstanced.WorldInvTranspose = trpInstanced.World.Inverse.Transposed;
		instanceTransformBuf = deviceBundle->CreateImmutableBuffer(engine->GetPrimaryCommandQueue(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, (const uint8*)&trpInstanced, sizeof(trpInstanced), D3D12_RESOURCE_FLAG_NONE);
	}
	else
	{
		instanceTransformBuf = deviceBundle->CreateDynamicBuffer(sizeof(RaytracingInstanceTransform));
	}
}

PrimitiveSceneProxy::~PrimitiveSceneProxy()
{

}

void PrimitiveSceneProxy::Update()
{
	
}

void PrimitiveSceneProxy::UpdateTransform()
{
	if (Mobility == EComponentMobility::Static)
	{
		SE_LOG(LogRendering, Error, L"Primitive mobility is static only. Update transform is not defined.");
	}

	PrimitiveTransform = myPrimitiveComponent->ComponentTransform;

	void* ptr;
	HR(instanceTransformBuf->Map(0, nullptr, &ptr));

	RaytracingInstanceTransform trpInstanced;
	trpInstanced.World = PrimitiveTransform.Matrix;
	trpInstanced.WorldInvTranspose = trpInstanced.World.Inverse.Transposed;

	memcpy(ptr, &trpInstanced, sizeof(trpInstanced));
	instanceTransformBuf->Unmap(0, nullptr);
}

uint64 PrimitiveSceneProxy::GetInstanceTransformBuf() const
{
	return instanceTransformBuf->GetGPUVirtualAddress();
}