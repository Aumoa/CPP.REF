// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:LogSystem;

import std.core;
import :LogVerbosity;
import :LogCategory;
import :StringUtils;

using namespace std;

/// <summary>
/// Provide logging functions.
/// </summary>
export class LogSystem abstract final
{
	class fatal_exception : public exception
	{
		string _storage;

	public:
		fatal_exception(string_view message);
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
	static void Log(LogCategory& category, ELogVerbosity logVerbosity, wstring_view format, TArgs&&... args)
	{
		wstring message = std::format(format, forward<TArgs>(args)...);
		InternalLog(category, logVerbosity, message);
	}

private:
	static void InternalLog(LogCategory& category, ELogVerbosity logVerbosity, wstring& message);
};