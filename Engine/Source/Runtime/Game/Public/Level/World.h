// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "GameFramework/Actor.h"
#include "Level/WorldType.h"

DECLARE_LOG_CATEGORY(GAME_API, LogWorld);

class SLevel;
class SScene;
class SSlateApplication;
class SLocalPlayer;

/// <summary>
/// Represents game world that contains spawned actor, physically state and environment.
/// </summary>
class GAME_API SWorld : public SGameObject
{
	GENERATED_BODY(SWorld)

	friend class ActorIterator;

private:
	SPROPERTY(Level)
	SLevel* Level = nullptr;
	SPROPERTY(Scene)
	SScene* Scene = nullptr;
	EWorldType WorldType;

public:
	/// <summary>
	/// Initialize new <see cref="SWorld"/> instance.
	/// </summary>
	SWorld(EWorldType InWorldType);

	virtual void InitWorld();
	virtual void DestroyWorld();

	virtual SWorld* GetWorld() override;
	EWorldType GetWorldType();
	SScene* GetScene();
	SLocalPlayer* GetLocalPlayer();

	AActor* SpawnActor(SubclassOf<AActor> InActorClass);
	void DestroyActor(AActor* InActor);

	SLevel* OpenLevel(SubclassOf<SLevel> InLevelToOpen);
	SLevel* GetLevel();

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
};