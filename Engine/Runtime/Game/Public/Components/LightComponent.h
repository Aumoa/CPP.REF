// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneComponent.h"

class GAME_API LightComponent : public SceneComponent
{
public:
	using Super = SceneComponent;
	using This = LightComponent;

public:
	LightComponent();
	~LightComponent() override;
};