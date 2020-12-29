// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"

#include "Logging/LogCategoryBase.h"
#include "Logging/LogVerbosity.h"
#include "Logging/Logger.h"

extern LogCategoryBase LogTH;

#define TH_LOG(CategoryName, Verbosity, Message, ...) Logger::Log(CategoryName, ELogVerbosity::Verbosity, String::Format(Message, __VA_ARGS__))