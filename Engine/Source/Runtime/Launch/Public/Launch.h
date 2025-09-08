// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class Engine;
	class DynamicLibrary;
	class CommandLineParser;

	class LAUNCH_API Launch
	{
	private:
		std::unique_ptr<CommandLineParser> m_Args;
		RPtr<Engine> m_Engine;

	public:
		Launch(std::unique_ptr<CommandLineParser> args);
		virtual ~Launch() noexcept;

		virtual int32 StartApplication();

		static int32 GuardedMain(std::unique_ptr<CommandLineParser> args, const DynamicLibrary& apiSet);
	};
}