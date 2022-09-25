// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class StaticPrimitiveDrawingInterface;

class GAME_API PrimitiveSceneProxy
{
public:
	PrimitiveSceneProxy();
	virtual ~PrimitiveSceneProxy() noexcept;

	virtual void DrawStaticElements(StaticPrimitiveDrawingInterface* PDI) = 0;
};