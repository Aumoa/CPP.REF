// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:LogCategory;

import std.core;
import :Object;
import :PrimitiveTypes;
import :LogVerbosity;
import :FileReference;

using enum ELogVerbosity;

/// <summary>
/// Represents log category that file logging state, display state, etc...
/// </summary>
export class LogCategory : virtual public Object
{
	friend class LogSystem;

	static std::optional<FileReference> _file;
	std::wstring _name;

public:
	/// <summary>
	/// Initialize new <see cref="LogCategory"/> instance.
	/// </summary>
	/// <param name="categoryName"> The category name. </param>
	LogCategory(std::wstring_view categoryName);
	~LogCategory();

private:
	static std::wstring_view VerbosityToString(ELogVerbosity verbosity);

protected:
	/// <summary>
	/// Callback logging processed on <see cref="LogSystem"/> instance.
	/// </summary>
	/// <param name="logVerbosity"> The log verbosity. </param>
	/// <param name="message"> The log message. </param>
	virtual void OnLog(ELogVerbosity logVerbosity, std::wstring_view message);
};