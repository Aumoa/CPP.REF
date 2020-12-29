// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::Logging
{
	struct LogCategoryBase;
}

namespace SC::Runtime::Game
{
	enum class TickingGroup
	{
		PrePhysics,
		DuringPhysics,
		PostPhysics,
		PostUpdateWork
	};

	struct GAME_API TickFunction
	{
		static Logging::LogCategoryBase LogTicking;

		bool bCanEverTick : 1;
		TickingGroup TickGroup;
		std::chrono::duration<double> TickInterval;

		TickFunction();
		~TickFunction();

		virtual void ExecuteTick(std::chrono::duration<double> deltaTime) = 0;
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::TickFunction;
#endif