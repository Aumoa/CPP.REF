// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include <array>
#include <set>
#include "TSubclassOf.h"

class AActor;

enum class TickingGroup;
struct TickFunction;
class Level;
class Scene;

class GAME_API World : virtual public Object
{
public:
	using Super = Object;
	using This = World;

private:
	std::vector<TRefPtr<AActor>> actors;

	std::array<std::set<TickFunction*>, 4> tickGroups;
	TRefPtr<Level> currentLevel = nullptr;
	TRefPtr<Scene> scene;

public:
	World();
	~World() override;

	virtual void Tick(Seconds deltaTime);

	template<TIsBaseOf<AActor> T, class... TArgs> requires THasConstructor<T, TArgs...>
	inline T* SpawnActor(TArgs&&... constructor_args);
	template<TIsBaseOf<AActor> T = AActor>
	inline T* SpawnActor(TSubclassOf<T> static_class);

	Level* LoadLevel(TSubclassOf<Level> loadLevel);

	Scene* GetScene() const;

private:
	void Tick_Group(Seconds deltaTime, TickingGroup group);
	AActor* SpawnActorInternal(TRefPtr<AActor> actor);

	void AddTickGroup(AActor* actor_ptr);
	void AddSceneProxy(AActor* actor_ptr);
};

#include "World.inl"