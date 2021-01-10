// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"
#include "Framework/GameModeBase.h"

class TH_API ATHGameMode : public AGameModeBase
{
public:
	using Super = AGameModeBase;
	using This = ATHGameMode;

public:
	ATHGameMode();
	~ATHGameMode() override;
};