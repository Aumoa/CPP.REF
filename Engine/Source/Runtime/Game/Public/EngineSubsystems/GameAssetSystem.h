// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"

class GAME_API GameAssetSystem : public GameEngineSubsystem
{
public:
	using Super = GameEngineSubsystem;

public:
	GameAssetSystem();
	virtual ~GameAssetSystem() override;

	virtual void Init() override;
};