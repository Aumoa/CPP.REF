// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

template<class... TArgs>
inline void Logger::Log(LogCategoryBase& category, ELogVerbosity inVerbosity, TRefPtr<String> logFormat, TArgs... args)
{
	Log(category, inVerbosity, String::Format(logFormat, args...));
}