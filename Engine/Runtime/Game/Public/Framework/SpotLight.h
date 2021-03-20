// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Actor.h"

class GSpotLightComponent;

class GAME_API ASpotLight : public AActor
{
public:
	using Super = AActor;
	using This = ASpotLight;

private:
	GSpotLightComponent* lightComponent;

public:
	ASpotLight();
	~ASpotLight() override;

	vs_property_get_auto(GSpotLightComponent*, LightComponent, lightComponent);
};