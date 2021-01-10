// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Logging/Logger.h"
#include "Logging/LogVerbosity.h"
#include "Logging/EngineLogCategory.h"

#define SE_LOG(CategoryName, Verbosity, Message, ...) Logger::Log(CategoryName, ELogVerbosity::Verbosity, String::Format(Message, __VA_ARGS__))