// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/PrimitiveSceneProxy.h"

#include "Engine.h"
#include "Components/PrimitiveComponent.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXDynamicBufferAllocator.h"
#include "Logging/LogMacros.h"
#include "GameplayStatics/ClassDependencyHelper.h"
#include "Shaders/ShaderTypes.h"

PrimitiveSceneProxy::PrimitiveSceneProxy(GPrimitiveComponent* inPrimitiveComponent) : Super()
	, Component(inPrimitiveComponent)
	, Mobility(inPrimitiveComponent->Mobility)
	, PrimitiveId(0)
	, PrimitiveBoundingBox{ }
	, PrimitiveAccelerationPtr(0)
{
	Engine* engine = ClassDependencyHelper::GetEngine(inPrimitiveComponent);
	DirectXDeviceBundle* deviceBundle = engine->GetDeviceBundle();
	DirectXDynamicBufferAllocator* allocator = deviceBundle->GetOrCreateDynamicBufferAllocator(sizeof(ShaderTypes::RaytracingInstanceTransform));
	instanceTransformBuf = NewObject<DirectXDynamicBuffer>(allocator);

	if (Mobility == EComponentMobility::Static)
	{
		PrimitiveTransform = inPrimitiveComponent->ComponentTransform;

		ShaderTypes::RaytracingInstanceTransform trpInstanced;
		trpInstanced.World = PrimitiveTransform.Matrix;
		trpInstanced.WorldInvTranspose = trpInstanced.World.Inverse.Transposed;

		memcpy(instanceTransformBuf->GetBufferPointer(), &trpInstanced, sizeof(trpInstanced));
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
		SE_LOG(LogRendering, Error, L"You try to update transform, but owning component is static mobility. The action does not defined.");
		return;
	}

	PrimitiveTransform = Component->ComponentTransform;

	ShaderTypes::RaytracingInstanceTransform trpInstanced;
	trpInstanced.World = PrimitiveTransform.Matrix;
	trpInstanced.WorldInvTranspose = trpInstanced.World.Inverse.Transposed;
	memcpy(instanceTransformBuf->GetBufferPointer(), &trpInstanced, sizeof(trpInstanced));
}

uint64 PrimitiveSceneProxy::GetInstanceTransformBuf() const
{
	return instanceTransformBuf->GetGPUVirtualAddress();
}