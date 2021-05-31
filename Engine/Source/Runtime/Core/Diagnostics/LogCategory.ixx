// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:LogCategory;

import std.core;
import :PrimitiveTypes;
import :LogVerbosity;

using enum ELogVerbosity;

/// <summary>
/// Represents log category that file logging state, display state, etc...
/// </summary>
export class LogCategory
{
	friend class LogSystem;

	std::wstring _name;
	std::wofstream _file;
	uint8 _logfile : 1 = false;

public:
	/// <summary>
	/// Initialize new <see cref="LogCategory"/> instance.
	/// </summary>
	/// <param name="categoryName"> The category name. </param>
	LogCategory(std::wstring_view categoryName);

private:
	static std::chrono::zoned_time<std::chrono::system_clock::duration> GetZonedTime();
	static std::wstring_view VerbosityToString(ELogVerbosity verbosity);
	static std::wstring GetTimedStringAppend(std::wstring_view name);

protected:
	/// <summary>
	/// Callback logging processed on <see cref="LogSystem"/> instance.
	/// </summary>
	/// <param name="logVerbosity"> The log verbosity. </param>
	/// <param name="message"> The log message. </param>
	virtual void OnLog(ELogVerbosity logVerbosity, std::wstring_view message);

	/// <summary>
	/// Get log file path that user overridden.
	/// </summary>
	/// <returns> Log file path. </returns>
	virtual std::filesystem::path GetLogFilePath() const;
};