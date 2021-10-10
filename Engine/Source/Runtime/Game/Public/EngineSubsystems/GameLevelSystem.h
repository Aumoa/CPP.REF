// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"
#include "SubclassOf.h"

class SLevel;
class SWorld;

class GAME_API SGameLevelSystem : public SGameEngineSubsystem
{
	GENERATED_BODY(SGameLevelSystem)

private:
	SWorld* _world = nullptr;
	SLevel* _loadedLevel = nullptr;

public:
	SGameLevelSystem();
	virtual ~SGameLevelSystem() override;

	virtual void Init() override;

	SWorld* GetWorld() const;
	bool OpenLevel(SubclassOf<SLevel> levelToLoad);
	SLevel* GetLevel() const;
};