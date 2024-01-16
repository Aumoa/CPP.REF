// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"

class ENGINE_API GameEngine : public GameObject
{
public:
	GameEngine();
	virtual ~GameEngine() noexcept;

	virtual void Initialize();
	virtual void Deinitialize();
};