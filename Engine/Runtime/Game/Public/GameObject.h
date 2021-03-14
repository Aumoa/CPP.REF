// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class World;

class GAME_API GGameObject : virtual public Object
{
public:
	using Super = Object;

private:
	World* world;
	TRefPtr<String> name;

public:
	GGameObject();
	~GGameObject() override;

	void SetWorld(World* value);
	World* GetWorld() const;

	void SetName(TRefPtr<String> value);
	TRefPtr<String> GetName() const;
};