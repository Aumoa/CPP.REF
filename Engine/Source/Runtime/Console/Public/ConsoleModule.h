// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/CommandLine.h"

class SConsoleModuleSubsystem;

class CONSOLE_API SConsoleModule : implements SObject
{
	GENERATED_BODY(SConsoleModule)

private:
	SPROPERTY(Subsystems)
	std::set<SConsoleModuleSubsystem*> Subsystems;

public:
	SConsoleModule();
	virtual ~SConsoleModule() noexcept override;

	int32 Main(const SCommandLine& CommandArgs);
	virtual int32 Run(const SCommandLine& CommandArgs) = 0;

	SConsoleModuleSubsystem* GetSubsystem(Type* SubsystemClass);
	template<class T>
	T* GetSubsystem()
	{
		return Cast<T>(GetSubsystem(T::StaticClass()));
	}

private:
	std::map<size_t, SConsoleModuleSubsystem*> CachedSubsystemView;
};

#define DEFINE_CONSOLE_MODULE(ConsoleModuleClass)			\
extern "C"													\
{															\
	__declspec(dllexport)									\
	SConsoleModule* LoadConsoleModule()						\
	{														\
		return SObject::NewObject<ConsoleModuleClass>();	\
	}														\
}