// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class GAME_API GameViewport : virtual public Object
{
	friend class Engine;

public:
	using Super = Object;
	using This = GameViewport;

private:
	int32 resX;
	int32 resY;

public:
	GameViewport();
	~GameViewport() override;

	vs_property_get(int32, ResolutionX);
	int32 ResolutionX_get() const;
	vs_property_get(int32, ResolutionY);
	int32 ResolutionY_get() const;

private:
	void SetViewportResolution_Internal(int32 x, int32 y);
};