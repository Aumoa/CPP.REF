// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/LightComponent.h"

#include "SceneRendering/LightSceneProxy.h"

LightComponent::LightComponent() : Super()
	, lightColor(Color::White)
	, lightAmbient(1.0f)
	, lightDiffuse(1.0f)
	, lightSpecular(1.0f)
	, bShadowCast(false)
	, bHasDirtyMark(true)
{

}

LightComponent::~LightComponent()
{

}

void LightComponent::ResolveDirtyState()
{
	if (HasDirtyMark(EComponentDirtyMask::RecreateProxy))
	{
		sceneProxy = CreateSceneProxy();
	}

	bool bHasSceneProxy = sceneProxy.IsValid;

	if (HasDirtyMark(EComponentDirtyMask::TransformUpdated) && bHasSceneProxy)
	{
		sceneProxy->UpdateMovable();
	}

	if (HasDirtyMark((EComponentDirtyMask)ELightComponentDirtyMask::LightUpdated) && bHasSceneProxy)
	{
		sceneProxy->UpdateBatchBuffer();
	}

	Super::ResolveDirtyState();
}

TRefPtr<LightSceneProxy> LightComponent::CreateSceneProxy()
{
	return nullptr;
}

LightSceneProxy* LightComponent::GetSceneProxy() const
{
	return sceneProxy.Get();
}

Color LightComponent::LightColor_get() const
{
	return lightColor;
}

void LightComponent::LightColor_set(const Color& value)
{
	lightColor = value;
	SetMarkDirtyLightUpdated();
}

float LightComponent::Ambient_get() const
{
	return lightAmbient;
}

void LightComponent::Ambient_set(float value)
{
	lightAmbient = value;
	SetMarkDirtyLightUpdated();
}

float LightComponent::Diffuse_get() const
{
	return lightDiffuse;
}

void LightComponent::Diffuse_set(float value)
{
	lightDiffuse = value;
	SetMarkDirtyLightUpdated();
}

float LightComponent::Specular_get() const
{
	return lightSpecular;
}

void LightComponent::Specular_set(float value)
{
	lightSpecular = value;
	SetMarkDirtyLightUpdated();
}

bool LightComponent::IsShadowCast_get() const
{
	return bShadowCast;
}

void LightComponent::IsShadowCast_set(bool value)
{
	bShadowCast = true;
	SetMarkDirtyLightUpdated();
}

void LightComponent::SetMarkDirtyLightUpdated()
{
	Super::SetMarkDirty((EComponentDirtyMask)ELightComponentDirtyMask::LightUpdated);
}