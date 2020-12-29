// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#define __SC_GLOBAL_NAMESPACE__

#ifdef __TH_API__
#define TH_API __declspec(dllexport)

#include "Logging/Logger.h"

#define TH_LOG(CategoryName, Verbosity, Message, ...) SC::Runtime::Game::Logging::Logger::Log(CategoryName, SC::Runtime::Game::Logging::ELogVerbosity::Verbosity, SC::Runtime::Core::String::Format(Message, __VA_ARGS__))

#else
#define TH_API __declspec(dllimport)
#endif