// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SubclassOf.h"
#include "World.h"

class SWorld;
class STickTaskLevelManager;
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
	STickTaskLevelManager* _LevelTick = nullptr;

	AGameMode* _GameMode = nullptr;
	APlayerController* _PlayerController = nullptr;

public:
	std::vector<AActor*> Actors;
	std::vector<AActor*> ActorsToAdd;
	std::vector<AActor*> ActorsToRemove;

public:
	/// <summary>
	/// Initialize new <see cref="SLevel"/> instance.
	/// </summary>
	SLevel();
	~SLevel();

	virtual bool LoadLevel(SWorld* InWorld, STickTaskLevelManager* InParentLevelTick = nullptr);
	virtual void UnloadLevel();

	void IncrementalActorsApply(size_t InLimit = 10);

	APlayerController* GetPlayerController();
	SWorld* GetWorld();
	STickTaskLevelManager* GetLevelTick();

	void InternalRemoveActor(AActor* InActor);
	void InternalAddActor(AActor* InActor);
};