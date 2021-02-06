// Copyright 2020 Aumoa.lib. All right reserved.

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

TRefPtr<LightSceneProxy> LightComponent::CreateSceneProxy()
{
	return nullptr;
}

LightSceneProxy* LightComponent::GetSceneProxy() const
{
	return sceneProxy.Get();
}

void LightComponent::SetMarkDirty()
{
	bHasDirtyMark = true;
}

bool LightComponent::HasDirtyMark() const
{
	return bHasDirtyMark;
}

void LightComponent::ResolveDirtyState()
{
	sceneProxy = CreateSceneProxy();
	bHasDirtyMark = false;
}