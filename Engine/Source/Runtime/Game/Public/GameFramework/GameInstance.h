// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "GameInstance.gen.h"

class GameEngine;
class Level;
class World;

SCLASS()
class GAME_API GameInstance : public GameObject
{
	GENERATED_BODY()

	friend class GameApplication;

protected:
	SubclassOf<Level> StartupLevel;

public:
	GameInstance();

	void Start(World* Browser);
	GameEngine* GetEngine() noexcept;

protected:
	virtual void Init();
	virtual void Deinit();
};