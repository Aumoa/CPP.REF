// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player.h"

class LocalPlayer : public Player
{
public:
	using Super = Player;

public:
	LocalPlayer();
};