// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SubclassOf.h"
#include "World.h"

class World;
class AGameMode;
class APlayerController;
class AActor;

/// <summary>
/// Represents actor placement unit.
/// </summary>
class GAME_API SLevel : virtual public SObject
{
	GENERATED_BODY(SLevel)

public:
	SubclassOf<AGameMode> GameModeClass;

private:
	World* _world = nullptr;
	APlayerController* _playerController = nullptr;
	AGameMode* _gameMode = nullptr;

	std::vector<AActor*> _persistentActors;

public:
	/// <summary>
	/// Initialize new <see cref="SLevel"/> instance.
	/// </summary>
	SLevel();
	~SLevel();

	/// <summary>
	/// Load level.
	/// </summary>
	/// <param name="world"> The world that level be placed. </param>
	virtual bool LoadLevel(World* world);

	/// <summary>
	/// Unload level. Persistent actor will be cleaned up.
	/// </summary>
	virtual void UnloadLevel();

	inline APlayerController* GetPlayerController() const { return _playerController; }
	inline World* GetWorld() const { return _world; }

protected:
	template<class... TArgs>
	AActor* SpawnActor(TArgs&&... args)
	{
		AActor* spawnActor = _world->SpawnActor(std::forward<TArgs>(args)...);
		InternalSpawnActor(spawnActor);
		return spawnActor;
	}

private:
	void InternalSpawnActor(AActor* actor);
};