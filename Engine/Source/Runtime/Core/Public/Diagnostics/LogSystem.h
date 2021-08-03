// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <exception>
#include <string>
#include <string_view>
#include <format>

class LogCategory;
enum class ELogVerbosity;

/// <summary>
/// Provide logging functions.
/// </summary>
class CORE_API LogSystem abstract final
{
	class fatal_exception : public std::exception
	{
	public:
		fatal_exception(const std::string& message);
	};

public:
	/// <summary>
	/// Log message.
	/// </summary>
	/// <typeparam name="...TArgs"> Type of formatter args. </typeparam>
	/// <param name="category"> The log category. </param>
	/// <param name="logVerbosity"> The log verbosity. </param>
	/// <param name="format"> The text format. </param>
	/// <param name="...args"> The formatter args. </param>
	template<class... TArgs>
	static void Log(LogCategory& category, ELogVerbosity logVerbosity, std::wstring_view format, TArgs&&... args)
	{
		std::wstring message = std::format(format, std::forward<TArgs>(args)...);
		InternalLog(category, logVerbosity, message);
	}

private:
	static void InternalLog(LogCategory& category, ELogVerbosity logVerbosity, std::wstring& message);
};