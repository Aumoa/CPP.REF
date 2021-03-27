// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include <array>
#include <set>
#include "TSubclassOf.h"

enum class TickingGroup;
class GameInstance;
class GLevel;
class AActor;
class APlayerController;
class Scene;
struct TickFunction;
class GActorComponent;

class GAME_API World : virtual public Object
{
public:
	using Super = Object;
	using This = World;

private:
	GameInstance* gameInstance;

	std::vector<TRefPtr<AActor>> actors;

	std::array<std::set<TickFunction*>, 4> tickGroups;
	std::array<std::set<TickFunction*>, 4> actualTickGroups;
	TRefPtr<GLevel> currentLevel;
	TRefPtr<Scene> scene;
	APlayerController* localPlayerController;

public:
	World(GameInstance* gameInstance);
	~World() override;

	virtual void Tick(Seconds deltaTime);

	template<TIsBaseOf<AActor> T, class... TArgs> requires THasConstructor<T, TArgs...>
	inline T* SpawnActor(TArgs&&... constructor_args);
	template<TIsBaseOf<AActor> T = AActor>
	inline T* SpawnActor(TSubclassOf<T> static_class);
	void RegisterComponent(GActorComponent* inComponent);

	GLevel* LoadLevel(TSubclassOf<GLevel> loadLevel);

	GameInstance* GetGameInstance() const;
	Scene* GetScene() const;
	GLevel* GetCurrentLevel() const;

private:
	void Tick_Ready();
	void Tick_Group(Seconds deltaTime, TickingGroup group);
	AActor* SpawnActorInternal(TRefPtr<AActor> actor);

	void AddTickGroup(AActor* actor_ptr);
	void RegisterActorComponents(AActor* actor_ptr);

	void OnLoadLevel();
	void OnUnloadLevel();

private:
	friend class GameInstance;

	void RegisterPlayerController(APlayerController* inPlayerController);
};

#include "World.inl"