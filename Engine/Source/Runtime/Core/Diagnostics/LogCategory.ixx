// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:LogCategory;

import std.core;
import :LogVerbosity;

/// <summary>
/// Represents log category that file logging state, display state, etc...
/// </summary>
export class LogCategory
{
	friend class LogSystem;

	std::wstring _name;

public:
	/// <summary>
	/// Initialize new <see cref="LogCategory"/> instance.
	/// </summary>
	/// <param name="categoryName"> The category name. </param>
	LogCategory(std::wstring_view categoryName)
	{
		_name = categoryName;
	}

protected:
	/// <summary>
	/// Callback logging processed on <see cref="LogSystem"/> instance.
	/// </summary>
	/// <param name="logVerbosity"> The log verbosity. </param>
	/// <param name="message"> The log message. </param>
	virtual void OnLog(ELogVerbosity logVerbosity, std::wstring_view message)
	{
	}
};