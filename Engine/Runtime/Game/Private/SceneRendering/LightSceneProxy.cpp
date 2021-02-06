// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/LightSceneProxy.h"

#include "Engine.h"
#include "Components/LightComponent.h"
#include "RHI/IRHIDeviceBundle.h"
#include "RHI/RHICommon.h"
#include "RHI/IRHIResource.h"
#include "SceneRendering/MinimalViewInfo.h"

LightBatch::LightBatch()
{
	GEngine.DeviceBundle->CreateDynamicConstantBuffer(sizeof(RHILight));
}

LightBatch::~LightBatch()
{

}

TRefPtr<IRHIResource> LightBatch::GetLightBuffer() const
{
	return basicLightBuffer;
}

LightSceneProxy::LightSceneProxy(LightComponent* inLightComponent) : Super()
	, myLightComponent(inLightComponent)
	, transform(Transform::Identity)
{
	UpdateMovable();
}

LightSceneProxy::~LightSceneProxy()
{

}

void LightSceneProxy::UpdateMovable()
{
	transform = myLightComponent->ComponentTransform;
	UpdateBatchBuffer();
}

Transform LightSceneProxy::GetLightTransform() const
{
	return transform;
}

LightBatch* LightSceneProxy::GetLightBatch() const
{
	return nullptr;
}

void LightSceneProxy::UpdateBatchBuffer()
{
	auto lightBatch = GetLightBatch();
	RHILight& light = *(RHILight*)lightBatch->GetLightBuffer()->GetMappingAddress();

	MinimalViewInfo viewInfo;
	myLightComponent->CalcLightView(viewInfo);

	light.Ambient = myLightComponent->Ambient;
	light.Diffuse = myLightComponent->Diffuse;
	light.Specular = myLightComponent->Specular;
	light.ShadowCast = (uint32)myLightComponent->IsShadowCast;
	light.ViewProj = viewInfo.ViewProj;
}