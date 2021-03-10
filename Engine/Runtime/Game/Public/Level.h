// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "GameObject.h"

#include "World.h"

class AActor;
class World;

class GAME_API GLevel : public GGameObject
{
	friend class World;

public:
	using Super = Object;
	using This = GLevel;

public:
	GLevel();
	~GLevel() override;

	virtual void LoadLevel();

protected:
	template<class T, class... TArgs> requires TIsAssignable<T*, AActor*> && THasConstructor<T, TArgs...>
	inline T* SpawnActorPersistent(TArgs&&... constructor_args);

private:
	void UnloadLevel();
};

#include "Level.inl"