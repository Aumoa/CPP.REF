// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include <array>
#include <set>
#include "TSubclassOf.h"

enum class TickingGroup;
interface IRHIScene;
class Level;
class AActor;
class APlayerController;
struct TickFunction;

class GAME_API World : virtual public Object
{
public:
	using Super = Object;
	using This = World;

private:
	std::vector<TRefPtr<AActor>> actors;

	std::array<std::set<TickFunction*>, 4> tickGroups;
	std::array<std::set<TickFunction*>, 4> actualTickGroups;
	TRefPtr<Level> currentLevel;
	TRefPtr<IRHIScene> scene;
	APlayerController* localPlayerController;

public:
	World();
	~World() override;

	virtual void Tick(Seconds deltaTime);

	template<TIsBaseOf<AActor> T, class... TArgs> requires THasConstructor<T, TArgs...>
	inline T* SpawnActor(TArgs&&... constructor_args);
	template<TIsBaseOf<AActor> T = AActor>
	inline T* SpawnActor(TSubclassOf<T> static_class);

	Level* LoadLevel(TSubclassOf<Level> loadLevel);

	IRHIScene* GetScene() const;
	Level* GetCurrentLevel() const;

private:
	void Tick_Ready();
	void Tick_Group(Seconds deltaTime, TickingGroup group);
	AActor* SpawnActorInternal(TRefPtr<AActor> actor);

	void AddTickGroup(AActor* actor_ptr);
	void AddSceneProxy(AActor* actor_ptr);

	void OnLoadLevel();
	void OnUnloadLevel();

private:
	friend class GameInstance;

	void RegisterPlayerController(APlayerController* inPlayerController);
};

#include "World.inl"