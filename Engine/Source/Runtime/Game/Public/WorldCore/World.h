// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "World.generated.h"

class LevelTick;
class GameEngine;
class Level;
class AActor;
class RenderScene;

SCLASS()
class GAME_API World : public GameObject
{
	GENERATED_BODY()

private:
	SPROPERTY()
	LevelTick* LevelTick = nullptr;

	SPROPERTY()
	Level* PersistentLevel = nullptr;
	std::vector<RefPtr<AActor>> Actors;

	std::unique_ptr<RenderScene> ScenePrivate;

public:
	World();
	virtual ~World() noexcept override;

	void DispatchWorldTick(const TimeSpan& InDeltaTime);

	void BrowseLevel(SubclassOf<Level> InLevelClass);

	inline RenderScene* GetScene() noexcept { return ScenePrivate.get(); }

	AActor* SpawnActor(SubclassOf<AActor> InActorClass, const String& ActorName = TEXT(""));

	template<std::derived_from<AActor> UAActor>
	UAActor* SpawnActor(SubclassOf<UAActor> InActorClass, const String& ActorName = TEXT(""))
	{
		return Cast<UAActor>(SpawnActor((SubclassOf<AActor>)InActorClass, ActorName));
	}

	template<std::derived_from<AActor> UAActor>
	UAActor* SpawnActor(const String& ActorName = TEXT(""))
	{
		return SpawnActor(UAActor::StaticClass(), ActorName);
	}

	GameEngine* GetEngine() noexcept;
	::LevelTick* GetLevelTick() noexcept;
};