// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "World.h"

class AActor;
class World;

class GAME_API Level : virtual public Object
{
	friend class World;

public:
	using Super = Object;
	using This = Level;

private:
	World* world;

public:
	Level();
	~Level() override;

	virtual void LoadLevel();

protected:
	template<class T, class... TArgs> requires TIsAssignable<T*, AActor*> && THasConstructor<T, TArgs...>
	inline T* SpawnActorPersistent(TArgs&&... constructor_args);

private:
	void UnloadLevel();
};

#include "Level.inl"