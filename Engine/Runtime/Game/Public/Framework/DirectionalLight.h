// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Actor.h"

class DirectionalLightComponent;

class GAME_API ADirectionalLight : public AActor
{
public:
	using Super = AActor;
	using This = ADirectionalLight;

private:
	DirectionalLightComponent* lightComponent;

public:
	ADirectionalLight();
	~ADirectionalLight() override;

	vs_property_get_auto(DirectionalLightComponent*, LightComponent, lightComponent);
};