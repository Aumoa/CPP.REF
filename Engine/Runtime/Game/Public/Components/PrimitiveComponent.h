// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneComponent.h"

class GAME_API PrimitiveComponent : public SceneComponent
{
public:
	using Super = SceneComponent;
	using This = PrimitiveComponent;

public:
	PrimitiveComponent();
	~PrimitiveComponent() override;
};