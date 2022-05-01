// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Console
{
	class CONSOLE_API SConsoleApplication : implements(SObject)
	{
		GENERATED_BODY(SConsoleApplication);
		SConsoleApplication() = delete;

	public:
		static int32 GuardedMain(std::span<const std::wstring> Argv);

	public:
		DECLARE_MULTICAST_EVENT(SigintEvent);
		static SigintEvent Sigint;

	private:
		static void _Handle_sigint(int);
		static void _Console_logged(const LogEntry& entry);
	};
}