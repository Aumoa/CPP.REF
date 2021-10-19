// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "SubclassOf.h"
#include "GameFramework/Actor.h"
#include "Level/WorldType.h"

DECLARE_LOG_CATEGORY(GAME_API, LogWorld);

class SLevel;
class SScene;

/// <summary>
/// Represents game world that contains spawned actor, physically state and environment.
/// </summary>
class GAME_API SWorld : public SGameObject
{
	GENERATED_BODY(SWorld)

private:
	SLevel* _Level = nullptr;
	SScene* _Scene = nullptr;
	EWorldType _WorldType;

public:
	/// <summary>
	/// Initialize new <see cref="SWorld"/> instance.
	/// </summary>
	SWorld(EWorldType InWorldType);

	virtual void InitWorld();
	virtual void DestroyWorld();

	virtual SWorld* GetWorld() override;
	EWorldType GetWorldType();

	AActor* SpawnActor(SubclassOf<AActor> InActorClass, bool bSpawnIncremental = false);
	void DestroyActor(AActor* InActor);

	SLevel* OpenLevel(SubclassOf<SLevel> InLevelToOpen);
	SLevel* GetLevel();

	const std::vector<AActor*>& GetAllActors();
	std::vector<AActor*> GetAllActorsOfClass(SubclassOf<AActor> InClass);

	void LevelTick(float InDeltaTime);

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

	template<std::derived_from<AActor> T>
	std::set<T*> GetAllActorsOfClass()
	{
		std::set<T*> ActorsOfClass;
		for (auto& Actor : _Actors)
		{
			if (auto* Ptr = dynamic_cast<T*>(Actor); Ptr)
			{
				ActorsOfClass.emplace(Ptr);
			}
		}
		return ActorsOfClass;
	}
};