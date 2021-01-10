// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "TSubclassOf.h"

class World;
class AGameModeBase;
class APlayerController;

class GAME_API GameInstance : virtual public Object
{
public:
	using Super = Object;
	using This = GameInstance;

private:
	static TRefPtr<String> defaultAppName;

	TRefPtr<World> world;
	AGameModeBase* gameMode;
	APlayerController* localPlayerController;

public:
	GameInstance();
	~GameInstance() override;

	TRefPtr<String> ToString() const override;

	virtual void Initialize();
	virtual void Tick(Seconds deltaTime);
	virtual void BeginPlay();
	virtual void EndPlay();

	World* GetWorld() const;
	AGameModeBase* GetGameMode() const;
	APlayerController* GetLocalPlayer() const;

	TSubclassOf<AGameModeBase> GameModeClass;
};