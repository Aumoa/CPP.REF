// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "PrimitiveComponent.h"

class GAME_API GMeshComponent : public GPrimitiveComponent
{
public:
	using Super = GPrimitiveComponent;
	using This = GPrimitiveComponent;

public:
	GMeshComponent();
	~GMeshComponent() override;
};