// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/CommandLine.h"

class CONSOLE_API SConsoleModule : implements SObject
{
	GENERATED_BODY(SConsoleModule)

public:
	SConsoleModule();

	virtual int32 Main(const SCommandLine& CommandArgs) = 0;
};

#define DEFINE_CONSOLE_MODULE(ConsoleModuleClass)			\
extern "C"													\
{															\
	__declspec(dllexport)									\
	SConsoleModule* LoadConsoleModule(SObject* Outer)		\
	{														\
		return Outer->NewObject<ConsoleModuleClass>();		\
	}														\
}