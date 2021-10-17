// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"
#include "SubclassOf.h"
#include "Level/WorldType.h"

class SLevel;
class SWorld;

class GAME_API SGameLevelSystem : public SGameEngineSubsystem
{
	GENERATED_BODY(SGameLevelSystem)

private:
	SWorld* _GameWorld = nullptr;
	SLevel* _LoadedLevel = nullptr;

public:
	SGameLevelSystem();
	virtual ~SGameLevelSystem() override;

	virtual void Init() override;
	virtual void Deinit() override;

	SWorld* SpawnWorld(EWorldType InWorldType);

	SWorld* GetGameWorld() const;
	bool OpenLevel(SubclassOf<SLevel> InLevelToLoad);
	SLevel* GetLevel() const;
};