// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneRenderer.h"

class GAME_API RaytracingSceneRenderer : public SceneRenderer
{
public:
	RaytracingSceneRenderer();

	virtual void Render(SceneView* Scene) override;
};