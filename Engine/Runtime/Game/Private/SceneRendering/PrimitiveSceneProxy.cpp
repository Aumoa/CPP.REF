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
	, myPrimitiveComponent(inPrimitiveComponent)

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
		SE_LOG(LogRendering, Error, L"Primitive mobility is static only. Update transform is not defined.");
	}

	PrimitiveTransform = myPrimitiveComponent->ComponentTransform;

	void* ptr = instanceTransformBuf->GetBufferPointer();

	ShaderTypes::RaytracingInstanceTransform trpInstanced;
	trpInstanced.World = PrimitiveTransform.Matrix;
	trpInstanced.WorldInvTranspose = trpInstanced.World.Inverse.Transposed;

	memcpy(ptr, &trpInstanced, sizeof(trpInstanced));
}

uint64 PrimitiveSceneProxy::GetInstanceTransformBuf() const
{
	return instanceTransformBuf->GetGPUVirtualAddress();
}