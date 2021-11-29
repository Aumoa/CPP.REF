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

	bool LoadLevel(SWorld* InWorld, STickTaskLevelManager* InParentLevelTick = nullptr);
	void UnloadLevel();

	AActor* SpawnActor(SubclassOf<AActor> InActorClass, bool bSpawnIncremental = true);
	void DestroyActor(AActor* InActor);
	void IncrementalActorsApply(size_t InLimit = 10);

	APlayerController* GetPlayerController();
	SWorld* GetWorld();
	STickTaskLevelManager* GetLevelTick();

	void InternalRemoveActor(AActor* InActor, bool bRemoveFromArray = false);
	void InternalAddActor(AActor* InActor);

protected:
	virtual void OnLoadLevel() {};
	virtual void OnUnloadLevel() {};

public:
	template<std::derived_from<AActor> T>
	T* SpawnActor()
	{
		return SpawnActor<T>(T::StaticClass());
	}

	template<std::derived_from<AActor> T>
	T* SpawnActor(SubclassOf<T> InActorClass)
	{
		return static_cast<T*>(SpawnActor((SubclassOf<AActor>)InActorClass));
	}
};