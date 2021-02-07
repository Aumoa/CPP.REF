﻿// Copyright 2020 Aumoa.lib. All right reserved.

#include "Framework/DirectionalLight.h"

#include "Components/DirectionalLightComponent.h"

ADirectionalLight::ADirectionalLight() : Super()
	, lightComponent(nullptr)
{
	lightComponent = AddComponent<DirectionalLightComponent>();
	RootComponent = lightComponent;
}

ADirectionalLight::~ADirectionalLight()
{

}