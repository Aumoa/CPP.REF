// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include <optional>
#include <string>
#include <string_view>
#include "IO/FileReference.h"

enum class ELogVerbosity;

/// <summary>
/// Represents log category that file logging state, display state, etc...
/// </summary>
class CORE_API LogCategory : virtual public Object
{
	CLASS_BODY(LogCategory)
	friend class LogSystem;

private:
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

#define DECLARE_LOG_CATEGORY(API, CategoryName) extern API LogCategory CategoryName;
#define DEFINE_LOG_CATEGORY(CategoryName) LogCategory CategoryName(L ## #CategoryName);