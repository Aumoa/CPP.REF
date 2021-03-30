// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Framework/SpotLight.h"

#include "Components/SpotLightComponent.h"

ASpotLight::ASpotLight() : Super()
	, lightComponent(nullptr)
{
	lightComponent = AddComponent<GSpotLightComponent>();
	RootComponent = lightComponent;
}

ASpotLight::~ASpotLight()
{

}