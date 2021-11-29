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
class CORE_API SLogCategory : implements SObject
{
	GENERATED_BODY(SLogCategory)
	friend class LogSystem;

private:
	static std::optional<SFileReference> _file;
	std::wstring _name;

public:
	/// <summary>
	/// Initialize new <see cref="SLogCategory"/> instance.
	/// </summary>
	/// <param name="categoryName"> The category name. </param>
	SLogCategory(std::wstring_view categoryName);
	~SLogCategory();

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

#define DECLARE_LOG_CATEGORY(API, CategoryName) extern API SLogCategory CategoryName;
#define DEFINE_LOG_CATEGORY(CategoryName) SLogCategory CategoryName(L ## #CategoryName);