// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Console.h"
#include "Action.h"
#include "Platform/PlatformIO.h"
#include <csignal>

namespace Ayla
{
	TextWriter& Console::Out = PlatformIO::GetStandardOutput();
	TextWriter& Console::Error = PlatformIO::GetStandardError();

	Console::CancelKeyPressedDelegate Console::CancelKeyPressed;

	inline namespace
	{
		Spinlock Lock;

		void BroadcastSignalInterrupt(int)
		{
			Console::CancelKeyPressed.Broadcast();
		}

		int SignalInit = +[]()
			{
				signal(SIGINT, BroadcastSignalInterrupt);
				return 0;
			}();
	}

	void Console::Write(String Str)
	{
		std::unique_lock ScopedLock(Lock);
		Out.Write(Str);
	}

	void Console::WriteLine(String Str)
	{
		std::unique_lock ScopedLock(Lock);
		Out.WriteLine(Str);
	}
}