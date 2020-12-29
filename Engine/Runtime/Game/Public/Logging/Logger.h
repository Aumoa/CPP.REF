// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::Logging
{
	enum class ELogVerbosity;
	struct LogCategoryBase;

	class GAME_API Logger : virtual public Core::Object
	{
	public:
		using Super = Core::Object;
		using This = Logger;

	private:
		Logger() = delete;

	public:
		static void Log(LogCategoryBase& category, ELogVerbosity inVerbosity, Core::TRefPtr<Core::String> logMessage);
		template<class... TArgs>
		inline static void Log(LogCategoryBase& category, ELogVerbosity inVerbosity, Core::TRefPtr<Core::String> logFormat, TArgs... args);

	private:
		static const wchar_t* ToString(ELogVerbosity inVerbosity);
	};
}

#include "Logger.inl"

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::Logging::Logger;
#endif