// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Delegates/MulticastEvent.h"
#include <exception>
#include <string>
#include <string_view>
#include <format>

class LogCategory;
enum class ELogVerbosity;

/// <summary>
/// Provide logging functions.
/// </summary>
class CORESOBJECT_API LogSystem
{
	using This = LogSystem;
	class SFatalException;

public:
	template<class... TArgs>
	static void Log(LogCategory& Category, ELogVerbosity LogVerbosity, std::wstring_view Format, TArgs&&... Args)
	{
		std::wstring Message = std::format(Format, std::forward<TArgs>(Args)...);
		InternalLog(Category, LogVerbosity, Message);
	}

private:
	static void InternalLog(LogCategory& Category, ELogVerbosity LogVerbosity, std::wstring& Message);
};

#define SE_LOG(Category, Verbosity, Format, ...) LogSystem::Log(Category, ELogVerbosity::Verbosity, Format __VA_OPT__(,) __VA_ARGS__)