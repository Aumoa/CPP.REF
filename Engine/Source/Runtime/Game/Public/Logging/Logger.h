// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

enum class ELogVerbosity;
struct LogCategoryBase;

class GAME_API Logger : virtual public Object
{
public:
	using Super = Object;
	using This = Logger;

private:
	Logger() = delete;

public:
	static void Log(LogCategoryBase& category, ELogVerbosity inVerbosity, TRefPtr<String> logMessage);
	template<class... TArgs>
	inline static void Log(LogCategoryBase& category, ELogVerbosity inVerbosity, TRefPtr<String> logFormat, TArgs... args);

private:
	static const wchar_t* ToString(ELogVerbosity inVerbosity);
};

#include "Logger.inl"