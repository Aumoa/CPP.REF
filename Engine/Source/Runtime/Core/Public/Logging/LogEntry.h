// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "LogLevel.h"
#include "Misc/String.h"
#include "Threading/Thread.h"

class LogCategory;

struct LogEntry
{
	const LogCategory* Category;
	Thread LogThread;
	ELogLevel Level;
	String Message;
};