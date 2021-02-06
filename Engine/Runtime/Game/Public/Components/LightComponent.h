// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneComponent.h"

struct MinimalViewInfo;
class LightSceneProxy;

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
	bool bHasDirtyMark : 1;

	TRefPtr<LightSceneProxy> sceneProxy;

public:
	LightComponent();
	~LightComponent() override;

	virtual TRefPtr<LightSceneProxy> CreateSceneProxy();
	virtual void CalcLightView(MinimalViewInfo& outViewInfo) const = 0;

	LightSceneProxy* GetSceneProxy() const;

	void SetMarkDirty();
	bool HasDirtyMark() const;
	virtual void ResolveDirtyState();

	vs_property_auto(Color, LightColor, lightColor);
	vs_property_auto(float, Ambient, lightAmbient);
	vs_property_auto(float, Diffuse, lightDiffuse);
	vs_property_auto(float, Specular, lightSpecular);
	vs_property_auto(bool, IsShadowCast, bShadowCast);
};