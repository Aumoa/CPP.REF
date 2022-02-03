// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"
#include "Level/WorldType.h"

class SLevel;
class SWorld;

class GAME_API SGameLevelSystem : public SGameEngineSubsystem
{
	GENERATED_BODY(SGameLevelSystem)

private:
	SWorld* _GameWorld = nullptr;

public:
	SGameLevelSystem();
	virtual ~SGameLevelSystem() override;

	virtual void PostInit() override;
	virtual void Deinit() override;

	SWorld* SpawnWorld(EWorldType InWorldType);

	SWorld* GetGameWorld() const;
	bool OpenLevel(SubclassOf<SLevel> InLevelToOpen);
};