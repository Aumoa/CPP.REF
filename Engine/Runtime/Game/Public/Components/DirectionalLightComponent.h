// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "LightComponent.h"
#include "SceneRendering/LightSceneProxy.h"

class GAME_API DirectionalLightComponent : public LightComponent
{
public:
	using Super = LightComponent;
	using This = DirectionalLightComponent;

public:
	DirectionalLightComponent();
	~DirectionalLightComponent() override;

	TRefPtr<LightSceneProxy> CreateSceneProxy() override;
	void CalcLightView(MinimalViewInfo& outViewInfo) const override;
};