// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class GAME_API DirectXSwapChain : virtual public Object
{
public:
	using Super = Object;

public:
	DirectXSwapChain();
	~DirectXSwapChain() override;
};