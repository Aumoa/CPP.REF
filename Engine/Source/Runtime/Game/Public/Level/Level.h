// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "World.h"

class SWorld;
class STickTaskLevelManager;
class AGameMode;
class APlayerController;
class AActor;

class GAME_API SLevel : public SGameObject
{
	GENERATED_BODY(SLevel)

public:
	SubclassOf<AGameMode> GameModeClass;

private:
	SPROPERTY(World)
	SWorld* World = nullptr;
	SPROPERTY(LevelTick)
	STickTaskLevelManager* LevelTick = nullptr;

	SPROPERTY(GameMode)
	AGameMode* GameMode = nullptr;
	SPROPERTY(PlayerController)
	APlayerController* PlayerController = nullptr;

public:
	SPROPERTY(Actors)
	std::vector<AActor*> Actors;
	SPROPERTY(ActorsToAdd)
	std::vector<AActor*> ActorsToAdd;
	SPROPERTY(ActorsToRemove)
	std::vector<AActor*> ActorsToRemove;

public:
	SLevel();

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