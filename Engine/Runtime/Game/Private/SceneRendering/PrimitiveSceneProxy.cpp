// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/PrimitiveSceneProxy.h"

#include "Components/PrimitiveComponent.h"
#include "DirectX/DirectXCommon.h"
#include "Logging/LogMacros.h"

PrimitiveSceneProxy::PrimitiveSceneProxy(GPrimitiveComponent* inPrimitiveComponent) : Super()
	, myPrimitiveComponent(inPrimitiveComponent)

	, Mobility(inPrimitiveComponent->Mobility)
	, PrimitiveId(0)
	, PrimitiveBoundingBox{ }
	, PrimitiveAccelerationPtr(0)
{
	if (Mobility == EComponentMobility::Static)
	{
		PrimitiveTransform = inPrimitiveComponent->ComponentTransform;
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
}