// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "LightComponent.h"

class GAME_API GPointLightComponent : public GLightComponent
{
public:
	using Super = GLightComponent;

private:
	float constant;
	float linear;
	float quad;

public:
	GPointLightComponent();
	~GPointLightComponent() override;

	TRefPtr<LightSceneProxy> CreateSceneProxy() override;

	vs_property(float, Constant);
	vs_property(float, Linear);
	vs_property(float, Quad);
};