// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/LightSceneProxy.h"

#include "Engine.h"
#include "Components/LightComponent.h"
#include "RHI/IRHIDeviceBundle.h"
#include "RHI/RHICommon.h"
#include "RHI/IRHIResource.h"
#include "SceneRendering/MinimalViewInfo.h"

LightBatch::LightBatch()
{
	basicLightBuffer = GEngine.DeviceBundle->CreateDynamicBuffer(sizeof(RHILight));
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
	batch = NewObject<LightBatch>();
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
	return batch.Get();
}

void LightSceneProxy::UpdateBatchBuffer()
{
	auto lightBatch = GetLightBatch();
	RHILight& light = *(RHILight*)lightBatch->GetLightBuffer()->GetMappingAddress();

	MinimalViewInfo viewInfo;
	myLightComponent->CalcLightView(viewInfo);

	light.LightColor = myLightComponent->LightColor.Cast<Vector3>();
	light.Ambient = myLightComponent->Ambient;
	light.Diffuse = myLightComponent->Diffuse;
	light.Specular = myLightComponent->Specular;
	light.ShadowCast = (uint32)myLightComponent->IsShadowCast;
	light.ViewProj = viewInfo.ViewProj;
}