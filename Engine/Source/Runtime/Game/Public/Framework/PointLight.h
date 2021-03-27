// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Actor.h"

class GPointLightComponent;

class GAME_API APointLight : public AActor
{
public:
	using Super = AActor;
	using This = APointLight;

private:
	GPointLightComponent* lightComponent;

public:
	APointLight();
	~APointLight() override;

	vs_property_get_auto(GPointLightComponent*, LightComponent, lightComponent);
};