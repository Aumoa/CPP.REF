// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SceneView;

class GAME_API SceneRenderer
{
protected:
	SceneRenderer();
	virtual ~SceneRenderer() noexcept;

public:
	virtual void Render(SceneView* Scene) = 0;
};