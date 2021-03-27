// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "LightComponent.h"

class GAME_API GSpotLightComponent : public GLightComponent
{
public:
	using Super = GLightComponent;

private:
	Radians cutoff;
	Radians outerCutoff;
	float constant;
	float linear;
	float quad;

public:
	GSpotLightComponent();
	~GSpotLightComponent() override;

	TRefPtr<LightSceneProxy> CreateSceneProxy() override;

	vs_property(Radians, CutOff);
	vs_property(Radians, OuterCutOff);
	vs_property(float, Constant);
	vs_property(float, Linear);
	vs_property(float, Quad);
};