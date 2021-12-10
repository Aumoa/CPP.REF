// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ConsoleModule.h"
#include "ConsoleModuleSubsystem.h"

GENERATE_BODY(SConsoleModule);

SConsoleModule::SConsoleModule() : Super()
{
}

SConsoleModule::~SConsoleModule() noexcept
{
}

int32 SConsoleModule::Main(const SCommandLine& CommandArgs)
{
	for (auto& SubsystemClass : Type::FindAllSubclass<SConsoleModuleSubsystem>())
	{
		if (!SubsystemClass->IsA<SConsoleModuleSubsystem>())
		{
			auto* Subsystem = *Subsystems.emplace(Cast<SConsoleModuleSubsystem>(SubsystemClass->Instantiate())).first;
			Subsystem->Init();
		}
	}

	int32 ReturnCode = Run(CommandArgs);

	// Cleanup objects.
	Subsystems.clear();
	GC().Collect();

	return ReturnCode;
}