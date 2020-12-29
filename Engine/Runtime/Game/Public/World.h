// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include <chrono>

class AActor;

enum class TickingGroup;
struct TickFunction;
class Level;

class GAME_API World : virtual public Object
{
public:
	using Super = Object;
	using This = World;

private:
	std::vector<TRefPtr<AActor>> actors;
	std::map<TickingGroup, std::vector<TickFunction*>> tickGroups;
	TWeakPtr<Level> currentLevel = nullptr;

public:
	World();
	~World() override;

	virtual void Tick(std::chrono::duration<double> deltaTime);

	template<class T, class... TArgs> requires TIsAssignable<T*, AActor*> && THasConstructor<T, TArgs...>
	inline T* SpawnActor(TArgs&&... constructor_args);

	void LoadLevel(Level* loadLevel);

private:
	void Tick_PrePhysics(std::chrono::duration<double> deltaTime);
	AActor* SpawnActorInternal(TRefPtr<AActor> actor);
};

#include "World.inl"