// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"

class GAME_API GameAssetSystem : public GameEngineSubsystem
{
	CLASS_BODY(GameAssetSystem)

public:
	GameAssetSystem();
	virtual ~GameAssetSystem() override;
};