// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "LogLevel.h"
#include "DateTime.h"
#include "Misc/String.h"

class LogCategory;

struct LogEntry
{
	DateTime LogDate;
	const LogCategory* Category = nullptr;
	String LogThread;
	ELogLevel Level;
	String Message;
};