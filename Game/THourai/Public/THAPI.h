// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#ifdef __TH_API__
#define TH_API __declspec(dllexport)

#include "Logging/Logger.h"

#define TH_LOG(CategoryName, Verbosity, Message, ...) Logger::Log(CategoryName, ELogVerbosity::Verbosity, String::Format(Message, __VA_ARGS__))

#else
#define TH_API __declspec(dllimport)
#endif