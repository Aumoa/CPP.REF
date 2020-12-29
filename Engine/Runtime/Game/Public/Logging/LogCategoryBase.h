// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::Logging
{
	enum class ELogVerbosity;

	struct GAME_API LogCategoryBase
	{
		friend class Logger;

	private:
		Core::TRefPtr<Core::String> CategoryName;
		ELogVerbosity BaseVerbosity;

	public:
		LogCategoryBase(ELogVerbosity inBaseVerbosity, Core::TRefPtr<Core::String> inCategoryName);
		~LogCategoryBase();

	private:
		ELogVerbosity GetAmendedVerbosity(ELogVerbosity verbosity) const;
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::Logging::LogCategoryBase;
#endif