// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneComponent.h"

class PrimitiveSceneProxy;

class GAME_API GPrimitiveComponent : public GSceneComponent
{
public:
	using Super = GSceneComponent;
	using This = GPrimitiveComponent;

private:
	TRefPtr<PrimitiveSceneProxy> sceneProxy;

public:
	GPrimitiveComponent();
	~GPrimitiveComponent() override;

	void ResolveDirtyState() override;

	virtual TRefPtr<PrimitiveSceneProxy> CreateSceneProxy();
	PrimitiveSceneProxy* GetSceneProxy() const;
};