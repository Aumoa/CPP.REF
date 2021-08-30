// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class GameEngine;

class GAME_API GameEngineSubsystem : virtual public Object
{
	CLASS_BODY(GameEngineSubsystem)

public:
	GameEngineSubsystem();
	virtual ~GameEngineSubsystem() override;

	virtual void Init() {}
	virtual void Tick(std::chrono::duration<float> elapsedTime) {}
};