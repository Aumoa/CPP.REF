// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <optional>
#include <string>
#include <string_view>

enum class ELogVerbosity;

class CORE_API LogCategory
{
	friend class LogSystem;

private:
	std::wstring CategoryName;

public:
	LogCategory(std::wstring_view CategoryName);

private:
	static std::wstring_view VerbosityToString(ELogVerbosity Verbosity);

protected:
	virtual void OnLog(ELogVerbosity Verbosity, std::wstring_view Message);
};

#define DECLARE_LOG_CATEGORY(API, CategoryName) extern API LogCategory CategoryName;
#define DEFINE_LOG_CATEGORY(CategoryName) LogCategory CategoryName(L ## #CategoryName);