// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include <chrono>
#include <array>
#include <set>

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
	TWeakPtr<Level> currentLevel = nullptr;
	TRefPtr<Scene> scene;

public:
	World();
	~World() override;

	virtual void Tick(Seconds deltaTime);

	template<class T, class... TArgs> requires TIsAssignable<T*, AActor*> && THasConstructor<T, TArgs...>
	inline T* SpawnActor(TArgs&&... constructor_args);

	void LoadLevel(Level* loadLevel);

	Scene* GetScene() const;

private:
	void Tick_Group(Seconds deltaTime, TickingGroup group);
	AActor* SpawnActorInternal(TRefPtr<AActor> actor);

	void AddTickGroup(AActor* actor_ptr);
	void AddSceneProxy(AActor* actor_ptr);
};

#include "World.inl"