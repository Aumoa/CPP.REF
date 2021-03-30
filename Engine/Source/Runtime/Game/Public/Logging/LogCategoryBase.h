// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

enum class ELogVerbosity;

struct GAME_API LogCategoryBase
{
	friend class Logger;

private:
	TRefPtr<String> CategoryName;
	ELogVerbosity BaseVerbosity;

public:
	LogCategoryBase(ELogVerbosity inBaseVerbosity, TRefPtr<String> inCategoryName);
	~LogCategoryBase();

private:
	ELogVerbosity GetAmendedVerbosity(ELogVerbosity verbosity) const;
};