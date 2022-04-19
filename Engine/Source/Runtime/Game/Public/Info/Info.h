// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "GameFramework/Actor.h"

namespace libty::inline Game
{
	/// <summary>
	/// Represent game information placed to world.
	/// </summary>
	class GAME_API AInfo : extends(AActor)
	{
		GENERATED_BODY(AInfo);

	public:
		AInfo();
	};
}