// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "LightComponent.h"

class GAME_API GDirectionalLightComponent : public GLightComponent
{
public:
	using Super = GLightComponent;

public:
	GDirectionalLightComponent();
	~GDirectionalLightComponent() override;

	TRefPtr<LightSceneProxy> CreateSceneProxy() override;
};