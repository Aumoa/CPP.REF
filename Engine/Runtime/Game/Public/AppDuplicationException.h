// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game
{
	class GAME_API AppDuplicationException : public Core::Exception
	{
	public:
		using Super = Core::Exception;
		using This = AppDuplicationException;

	public:
		AppDuplicationException();
		~AppDuplicationException() override;
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::AppDuplicationException;
#endif