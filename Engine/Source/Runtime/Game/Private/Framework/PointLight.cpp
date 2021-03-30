// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Framework/PointLight.h"

#include "Components/PointLightComponent.h"

APointLight::APointLight() : Super()
	, lightComponent(nullptr)
{
	lightComponent = AddComponent<GPointLightComponent>();
	RootComponent = lightComponent;
}

APointLight::~APointLight()
{

}