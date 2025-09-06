// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Launch.gen.h"

namespace Ayla
{
	class Engine;
	class DynamicLibrary;
	class CommandLineParser;

	ACLASS()
	class LAUNCH_API Launch : public Object
	{
		GENERATED_BODY()

	private:
		std::unique_ptr<CommandLineParser> m_Args;
		APROPERTY()
		PPtr<Engine> m_Engine;

	protected:
		Launch(std::unique_ptr<CommandLineParser> args);

	public:
		virtual ~Launch() noexcept override;

		AFUNCTION()
		virtual int32 StartApplication();

		static int32 GuardedMain(std::unique_ptr<CommandLineParser> args, DynamicLibrary& api);
	};
}