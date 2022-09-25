// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include <set>
#include "World.gen.h"

class LevelTick;
class GameEngine;
class Level;
class AActor;
class RenderScene;
class ActorComponent;

SCLASS()
class GAME_API World : public GameObject
{
	GENERATED_BODY()

private:
	SPROPERTY()
	LevelTick* LevelTick = nullptr;

	SPROPERTY()
	Level* PersistentLevel = nullptr;

	std::map<Level*, std::set<RefPtr<AActor>>, std::less<>> LevelActors;
	std::list<ActorComponent*> PendingRegisterComponents;

	std::unique_ptr<RenderScene> ScenePrivate;

public:
	World();
	virtual ~World() noexcept override;

	void DispatchWorldTick(const TimeSpan& InDeltaTime);

	void BrowseLevel(SubclassOf<Level> InLevelClass);

	inline RenderScene* GetScene() noexcept { return ScenePrivate.get(); }
	void GetAllActorsOfClass(std::vector<AActor*>& OutActors, SubclassOf<AActor> InActorType) noexcept;

	AActor* SpawnActorAt(Level* InLevel, SubclassOf<AActor> InActorClass, const String& ActorName = TEXT(""));
	void DestroyActor(AActor* InActor);

	inline AActor* SpawnActor(SubclassOf<AActor> InActorClass, const String& ActorName = TEXT(""))
	{
		return SpawnActorAt(PersistentLevel, InActorClass, ActorName);
	}

	template<std::derived_from<AActor> UAActor>
	UAActor* SpawnActor(SubclassOf<UAActor> InActorClass, const String& ActorName = TEXT(""))
	{
		return Cast<UAActor>(SpawnActor((SubclassOf<AActor>)InActorClass, ActorName));
	}

	template<std::derived_from<AActor> UAActor>
	UAActor* SpawnActorAt(Level* InLevel, SubclassOf<UAActor> InActorClass, const String& ActorName = TEXT(""))
	{
		return Cast<UAActor>(SpawnActorAt(InLevel, (SubclassOf<AActor>)InActorClass, ActorName));
	}

	template<std::derived_from<AActor> UAActor>
	UAActor* SpawnActor(const String& ActorName = TEXT(""))
	{
		return SpawnActor(UAActor::StaticClass(), ActorName);
	}

	template<std::derived_from<AActor> UAActor>
	UAActor* SpawnActorAt(Level* InLevel, const String& ActorName = TEXT(""))
	{
		return SpawnActorAt(InLevel, UAActor::StaticClass(), ActorName);
	}

	GameEngine* GetEngine() noexcept;
	::LevelTick* GetLevelTick() noexcept;

private:
	void IncrementalRegisterComponents(size_t NumRegisterComponents = 10);
};