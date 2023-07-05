// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.Diagnostics:LogEntry;

export import Core.System;
export import :LogLevel;

export struct LogEntry
{
	DateTime LogDate;
	String LogThread;
	ELogLevel Level;
	String Message;
};