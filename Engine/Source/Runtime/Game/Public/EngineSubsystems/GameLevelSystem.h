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
	SWorld* _World = nullptr;
	SLevel* _LoadedLevel = nullptr;

public:
	SGameLevelSystem();
	virtual ~SGameLevelSystem() override;

	virtual void Init() override;
	virtual void Deinit() override;

	SWorld* GetWorld() const;
	bool OpenLevel(SubclassOf<SLevel> levelToLoad);
	SLevel* GetLevel() const;
};