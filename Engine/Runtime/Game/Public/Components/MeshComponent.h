// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "PrimitiveComponent.h"

class GAME_API MeshComponent : public PrimitiveComponent
{
public:
	using Super = PrimitiveComponent;
	using This = PrimitiveComponent;

public:
	MeshComponent();
	~MeshComponent() override;
};