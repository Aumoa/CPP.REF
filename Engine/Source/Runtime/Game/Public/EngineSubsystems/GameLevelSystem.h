// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "GameEngineSubsystem.h"
#include "SubclassOf.h"

class Level;

class GAME_API GameLevelSystem : public GameEngineSubsystem
{
	GENERATED_BODY(GameLevelSystem)

private:
	World* _world = nullptr;
	Level* _loadedLevel = nullptr;

public:
	GameLevelSystem();
	virtual ~GameLevelSystem() override;

	virtual void Init() override;

	World* GetWorld() const;
	bool OpenLevel(SubclassOf<Level> levelToLoad);
	Level* GetLevel() const;
};