// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SubclassOf.h"
#include "World.h"

class SWorld;
class AGameMode;
class APlayerController;
class AActor;

/// <summary>
/// Represents actor placement unit.
/// </summary>
class GAME_API SLevel : implements SObject
{
	GENERATED_BODY(SLevel)

public:
	SubclassOf<AGameMode> GameModeClass;

private:
	SWorld* _World = nullptr;

	AGameMode* _GameMode = nullptr;
	APlayerController* _PlayerController = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="SLevel"/> instance.
	/// </summary>
	SLevel();
	~SLevel();

	virtual bool LoadLevel(SWorld* InWorld);
	virtual void UnloadLevel();

	APlayerController* GetPlayerController();
	SWorld* GetWorld();
};