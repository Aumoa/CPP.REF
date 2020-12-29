// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class SceneRenderer;

class GAME_API Scene : virtual public Object
{
public:
	Scene();
	~Scene() override;

	void Render(SceneRenderer* renderer);
};