// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "LightComponent.h"

class GAME_API DirectionalLight : public LightComponent
{
public:
	using Super = LightComponent;
	using This = DirectionalLight;

public:
	DirectionalLight();
	~DirectionalLight() override;
};