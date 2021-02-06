// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/LightComponent.h"

LightComponent::LightComponent() : Super()
{
	lightColor = Color::White;
	lightAmbient = 1.0f;
	lightDiffuse = 1.0f;
	lightSpecular = 1.0f;
	bShadowCast = false;
}

LightComponent::~LightComponent()
{

}