// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Logging/Logger.h"
#include "Logging/LogVerbosity.h"

#define SE_LOG(CategoryName, Verbosity, Message, ...) SC::Runtime::Game::Logging::Logger::Log(CategoryName, SC::Runtime::Game::Logging::ELogVerbosity::Verbosity, SC::Runtime::Core::String::Format(Message, __VA_ARGS__))