// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Framework/DirectionalLight.h"

#include "Components/DirectionalLightComponent.h"

ADirectionalLight::ADirectionalLight() : Super()
	, lightComponent(nullptr)
{
	lightComponent = AddComponent<GDirectionalLightComponent>();
	RootComponent = lightComponent;
}

ADirectionalLight::~ADirectionalLight()
{

}