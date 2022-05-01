// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ConsoleEx.h"
#include "InternalConsole.h"
#include "ConsolePin.h"

using namespace ::libty;

void ConsoleEx::Write(ConsolePin pin, std::wstring message)
{
	LogModule::Get()->EnqueueLogAction([pin, message]() mutable
	{
		InternalConsole::Write(pin, message);
	});
}

void ConsoleEx::Clear(ConsolePin pin)
{
	LogModule::Get()->EnqueueLogAction([pin]() mutable
	{
		InternalConsole::Clear(pin, true);
	});
}