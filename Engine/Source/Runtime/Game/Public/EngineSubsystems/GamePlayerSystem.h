// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"

class LocalPlayer;
struct IFrameworkView;

class GAME_API GamePlayerSystem : public GameEngineSubsystem
{
	CLASS_BODY(GamePlayerSystem);

private:
	LocalPlayer* _localPlayer = nullptr;

public:
	GamePlayerSystem();
	virtual ~GamePlayerSystem() override;

	virtual void Init() override;
	virtual void Tick(std::chrono::duration<float> elapsedTime) override;

	void SpawnLocalPlayer(IFrameworkView* frameworkView);
	LocalPlayer* GetLocalPlayer() const;
};