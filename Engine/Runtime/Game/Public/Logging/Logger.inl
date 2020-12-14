// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

namespace SC::Runtime::Game::Logging
{
	template<class... TArgs>
	inline void Logger::Log(LogCategoryBase& category, ELogVerbosity inVerbosity, Core::TRefPtr<Core::String> logFormat, TArgs... args)
	{
		Log(category, inVerbosity, logFormat);
	}
}