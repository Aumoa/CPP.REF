// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:LogSystem;

import std.core;
import :LogVerbosity;
import :LogCategory;

/// <summary>
/// Provide logging functions.
/// </summary>
export class LogSystem abstract final
{
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
		category.OnLog(logVerbosity, message);
	}
};