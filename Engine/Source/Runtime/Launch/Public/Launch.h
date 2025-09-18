// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Launch.gen.h"

namespace Ayla
{
	class Engine;
	class DynamicLibrary;
	class CommandLineParser;
	class ScriptingBackend;

	ACLASS()
	class LAUNCH_API Launch : public Object
	{
		GENERATED_BODY()

	private:
		std::unique_ptr<CommandLineParser> m_Args;

	public:
		Launch(std::unique_ptr<CommandLineParser> args);
		virtual ~Launch() noexcept;

		AFUNCTION()
		virtual int32 GuardedMain(String platform);

		static int32 Main(std::unique_ptr<CommandLineParser> args, String platform);
	};
}