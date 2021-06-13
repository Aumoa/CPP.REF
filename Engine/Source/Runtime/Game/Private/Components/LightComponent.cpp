﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/LightComponent.h"

#include "SceneRendering/LightSceneProxy.h"

GLightComponent::GLightComponent() : Super()
	, lightColor(Color::White)
	, lightAmbient(0.2f)
	, lightDiffuse(0.5f)
	, lightSpecular(0.4f)
	, bShadowCast(false)
	, bHasDirtyMark(true)
{
	SetMarkDirty(EComponentDirtyMask::RecreateProxy);
}

GLightComponent::~GLightComponent()
{

}

void GLightComponent::ResolveDirtyState()
{
	bool bRecreated = false;
	if (HasDirtyMark(EComponentDirtyMask::RecreateProxy))
	{
		sceneProxy = CreateSceneProxy();
		bRecreated = true;
	}

	const bool bHasSceneProxy = sceneProxy.IsValid;
	if (bHasSceneProxy && !bRecreated)
	{
		if (HasDirtyMark((EComponentDirtyMask)ELightComponentDirtyMask::LightUpdated) && bHasSceneProxy)
		{
			sceneProxy->Update();
		}

		if (HasDirtyMark(EComponentDirtyMask::TransformUpdated) && bHasSceneProxy)
		{
			sceneProxy->UpdateTransform();
		}
	}

	Super::ResolveDirtyState();
}

TRefPtr<LightSceneProxy> GLightComponent::CreateSceneProxy()
{
	return nullptr;
}

LightSceneProxy* GLightComponent::GetSceneProxy() const
{
	return sceneProxy.Get();
}

Color GLightComponent::LightColor_get() const
{
	return lightColor;
}

void GLightComponent::LightColor_set(Color value)
{
	lightColor = value;
	SetMarkDirtyLightUpdated();
}

float GLightComponent::Ambient_get() const
{
	return lightAmbient;
}

void GLightComponent::Ambient_set(float value)
{
	lightAmbient = value;
	SetMarkDirtyLightUpdated();
}

float GLightComponent::Diffuse_get() const
{
	return lightDiffuse;
}

void GLightComponent::Diffuse_set(float value)
{
	lightDiffuse = value;
	SetMarkDirtyLightUpdated();
}

float GLightComponent::Specular_get() const
{
	return lightSpecular;
}

void GLightComponent::Specular_set(float value)
{
	lightSpecular = value;
	SetMarkDirtyLightUpdated();
}

bool GLightComponent::IsShadowCast_get() const
{
	return bShadowCast;
}

void GLightComponent::IsShadowCast_set(bool value)
{
	bShadowCast = true;
	SetMarkDirtyLightUpdated();
}

void GLightComponent::SetMarkDirtyLightUpdated()
{
	Super::SetMarkDirty((EComponentDirtyMask)ELightComponentDirtyMask::LightUpdated);
}