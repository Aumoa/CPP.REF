// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneComponent.h"

class PrimitiveSceneProxy;

class PrimitiveComponent : public SceneComponent
{
public:
	using Super = SceneComponent;

public:
	PrimitiveComponent();

	virtual PrimitiveSceneProxy* CreateSceneProxy() { return nullptr; }
	PrimitiveSceneProxy* SceneProxy = nullptr;
};