// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "LogEntry.h"

class ILoggingExtension
{
public:
	virtual ~ILoggingExtension() noexcept = default;

	virtual void Installed() = 0;
	virtual void Uninstalled() = 0;

	virtual void TraceOne(LogEntry& entry) = 0;
	virtual void Flush() = 0;
};