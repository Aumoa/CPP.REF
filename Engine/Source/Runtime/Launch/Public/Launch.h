// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "LaunchOptions.h"
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

	public:
		Launch();
		virtual ~Launch() noexcept;

		AFUNCTION()
		virtual int32 GuardedMain(SharedPtr<LaunchOptions> options);

		static int32 Main(std::unique_ptr<CommandLineParser> args, String platform);
	};
}