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

	virtual void ResolveDirtyState();

	virtual TRefPtr<LightSceneProxy> CreateSceneProxy();
	virtual void CalcLightView(MinimalViewInfo& outViewInfo) const = 0;
	LightSceneProxy* GetSceneProxy() const;

	vs_property(Color, LightColor);
	Color LightColor_get() const;
	void LightColor_set(const Color& value);

	vs_property(float, Ambient);
	float Ambient_get() const;
	void Ambient_set(float value);

	vs_property(float, Diffuse);
	float Diffuse_get() const;
	void Diffuse_set(float value);

	vs_property(float, Specular);
	float Specular_get() const;
	void Specular_set(float value);

	vs_property(bool, IsShadowCast);
	bool IsShadowCast_get() const;
	void IsShadowCast_set(bool value);

private:
	void SetMarkDirtyLightUpdated();
};