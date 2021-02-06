// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneComponent.h"

struct MinimalViewInfo;

class GAME_API LightComponent : public SceneComponent
{
public:
	using Super = SceneComponent;
	using This = LightComponent;

private:
	Color lightColor;
	float lightAmbient;
	float lightDiffuse;
	float lightSpecular;
	bool bShadowCast : 1;

public:
	LightComponent();
	~LightComponent() override;

	virtual void CalcLightView(MinimalViewInfo& outViewInfo) const = 0;

	vs_property_auto(Color, LightColor, lightColor);
	vs_property_auto(float, Ambient, lightAmbient);
	vs_property_auto(float, Diffuse, lightDiffuse);
	vs_property_auto(float, Specular, lightSpecular);
	vs_property_auto(bool, IsShadowCast, bShadowCast);
};