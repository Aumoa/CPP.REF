// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneComponent.h"

class PrimitiveSceneProxy;

class GAME_API PrimitiveComponent : public SceneComponent
{
public:
	using Super = SceneComponent;
	using This = PrimitiveComponent;

private:
	TRefPtr<PrimitiveSceneProxy> sceneProxy;

public:
	PrimitiveComponent();
	~PrimitiveComponent() override;

	void ResolveDirtyState() override;

	virtual TRefPtr<PrimitiveSceneProxy> CreateSceneProxy();
	PrimitiveSceneProxy* GetSceneProxy() const;
};