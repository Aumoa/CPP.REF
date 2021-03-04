// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Windows/Application.h"

#include "TSubclassOf.h"

class Engine;
class World;
class AGameModeBase;
class APlayerController;

class GAME_API GameInstance : public Application
{
public:
	using Super = Application;

private:
	TRefPtr<String> appName;
	TRefPtr<Engine> engine;
	TRefPtr<World> world;

	AGameModeBase* gameMode;
	APlayerController* localPlayerController;

public:
	GameInstance();
	~GameInstance() override;

	TRefPtr<String> ToString() const override;
	void Tick() override;

	World* GetWorld() const;
	AGameModeBase* GetGameMode() const;
	APlayerController* GetLocalPlayer() const;

	TSubclassOf<AGameModeBase> GameModeClass;

	vs_property(TRefPtr<String>, AppName);
	TRefPtr<String> AppName_get() const;
	void AppName_set(TRefPtr<String> value);

protected:
	void Initialize() override;
	void Shutdown() override;
};