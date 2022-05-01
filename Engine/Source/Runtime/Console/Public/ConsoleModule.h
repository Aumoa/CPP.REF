// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Console
{
	class SConsoleModuleSubsystem;

	class CONSOLE_API SConsoleModule : virtual public SObject
	{
		GENERATED_BODY(SConsoleModule);

	private:
		SPROPERTY(_sp)
		SObjectFactory* _sp = nullptr;

	public:
		SConsoleModule();
		virtual ~SConsoleModule() noexcept override;

		int32 Main(const CommandLine& commandLine);
		virtual void Configure(IServiceCollection* collection) = 0;
	};
}

#if PLATFORM_DYNAMIC_LIBRARY

#define DEFINE_CONSOLE_MODULE(ConsoleModuleClass)				\
	extern "C"													\
	{															\
		__declspec(dllexport)									\
		::libty::Console::SConsoleModule* LoadConsoleModule()	\
		{														\
			return gcnew ::ConsoleModuleClass();				\
		}														\
	}

#else

#define DEFINE_CONSOLE_MODULE(ConsoleModuleClass)				\
	extern "C"													\
	{															\
		::libty::Console::SConsoleModule* LoadConsoleModule()	\
		{														\
			return gcnew ::ConsoleModuleClass();				\
		}														\
	}

#endif