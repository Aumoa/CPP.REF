// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include <set>
#include "SubclassOf.h"
#include "LogGame.h"
#include "GameFramework/Actor.h"

class SLevel;
class SScene;
class SGameEngine;
class APlayerController;
class APlayerCameraManager;
class STickFunction;

/// <summary>
/// Represents game world that contains spawned actor, physically state and environment.
/// </summary>
class GAME_API SWorld : public SGameObject
{
	GENERATED_BODY(SWorld)

private:
	std::set<AActor*> _actors;
	SLevel* _level = nullptr;
	std::set<STickFunction*> _tickInstances;
	SScene* _scene = nullptr;

	APlayerController* _playerController = nullptr;
	APlayerCameraManager* _playerCamera = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="SWorld"/> instance.
	/// </summary>
	SWorld();

	/// <summary>
	/// Spawn actor to world.
	/// </summary>
	/// <typeparam name="T"> The actor class. </typeparam>
	/// <returns> Spawned actor. </returns>
	template<std::derived_from<AActor> T>
	T* SpawnActor()
	{
		T* spawnedActor = NewObject<T>();
		if (!InternalSpawnActor(spawnedActor))
		{
			DestroySubobject(spawnedActor);
			return nullptr;
		}
		return spawnedActor;
	}

	/// <summary>
	/// Spawn actor to world.
	/// </summary>
	/// <param name="actorClass"> The actor class. </typeparam>
	/// <returns> Spawned actor. </returns>
	AActor* SpawnActor(SubclassOf<AActor> actorClass);

	/// <summary>
	/// Spawn actor to world.
	/// </summary>
	/// <param name="actorClass"> The actor class. </typeparam>
	/// <returns> Spawned actor. </returns>
	template<std::derived_from<AActor> T>
	T* SpawnActor(SubclassOf<T> actorClass)
	{
		return dynamic_cast<T*>(SpawnActor(SubclassOf<AActor>(actorClass)));
	}

	/// <summary>
	/// Load level.
	/// </summary>
	SLevel* LoadLevel(SubclassOf<SLevel> levelToLoad);

	void RegisterTickFunction(STickFunction* function);
	void RegisterComponent(SActorComponent* component);
	void UnregisterTickFunction(STickFunction* function);
	void UnregisterComponent(SActorComponent* component);

	virtual void LevelTick(std::chrono::duration<float> elapsedTime);
	SScene* GetScene() const { return _scene; }
	APlayerCameraManager* GetPlayerCamera() const { return _playerCamera; }
	SLevel* GetLevel() const { return _level; }

private:
	bool InternalSpawnActor(AActor* instance);
};