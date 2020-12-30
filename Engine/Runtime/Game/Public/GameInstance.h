// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include <chrono>

class World;
class Level;

class GAME_API GameInstance : virtual public Object
{
public:
	using Super = Object;
	using This = GameInstance;

private:
	static TRefPtr<String> defaultAppName;

	TRefPtr<World> world;

public:
	GameInstance();
	~GameInstance() override;

	TRefPtr<String> ToString() const override;

	virtual void Initialize();
	virtual void Tick(std::chrono::duration<double> deltaTime);
	virtual void BeginPlay();
	virtual void EndPlay();

	virtual Level* GetStartupLevel() = 0;

	World* GetWorld() const;
};