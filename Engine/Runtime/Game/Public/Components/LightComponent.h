// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneComponent.h"

struct MinimalViewInfo;
class LightSceneProxy;

enum class ELightComponentDirtyMask : uint32
{
	LightUpdated = (uint32)EComponentDirtyMask::Last
};

class GAME_API GLightComponent : public GSceneComponent
{
public:
	using Super = GSceneComponent;
	using This = GLightComponent;

private:
	Color lightColor;
	float lightAmbient;
	float lightDiffuse;
	float lightSpecular;
	bool bShadowCast : 1;
	bool bHasDirtyMark : 1;

	TRefPtr<LightSceneProxy> sceneProxy;

public:
	GLightComponent();
	~GLightComponent() override;

	virtual void ResolveDirtyState();

	virtual TRefPtr<LightSceneProxy> CreateSceneProxy();
	virtual void CalcLightView(MinimalViewInfo& outViewInfo) const = 0;
	LightSceneProxy* GetSceneProxy() const;

	vs_property(Color, LightColor);
	vs_property(float, Ambient);
	vs_property(float, Diffuse);
	vs_property(float, Specular);
	vs_property(bool, IsShadowCast);

private:
	void SetMarkDirtyLightUpdated();
};