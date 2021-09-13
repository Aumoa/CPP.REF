// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"

class SLocalPlayer;
struct IFrameworkView;

class GAME_API SGamePlayerSystem : public SGameEngineSubsystem
{
	GENERATED_BODY(SGamePlayerSystem);

private:
	SLocalPlayer* _localPlayer = nullptr;

public:
	SGamePlayerSystem();
	virtual ~SGamePlayerSystem() override;

	virtual void Tick(std::chrono::duration<float> elapsedTime) override;

	void SpawnLocalPlayer(IFrameworkView* frameworkView);
	SLocalPlayer* GetLocalPlayer() const;
};