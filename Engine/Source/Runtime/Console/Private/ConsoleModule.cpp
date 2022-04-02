// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ConsoleModule.h"
#include "ConsoleModuleSubsystem.h"

using namespace libty;
using namespace libty::Console;

SConsoleModule::SConsoleModule() : Super()
{
}

SConsoleModule::~SConsoleModule() noexcept
{
}

int32 SConsoleModule::Main(const CommandLine& CommandArgs)
{
	for (auto& SubsystemClass : SType::GetDerivedTypes(typeof(SConsoleModuleSubsystem)))
	{
		if (!SubsystemClass->IsA(typeof(SConsoleModuleSubsystem)))
		{
			auto* Subsystem = *Subsystems.emplace(Cast<SConsoleModuleSubsystem>(SubsystemClass->Instantiate())).first;
			Subsystem->Init();
		}
	}

	int32 ReturnCode = Run(CommandArgs);

	// Cleanup subsystems.
	for (auto& Subsystem : Subsystems)
	{
		Subsystem->Deinit();
	}

	Subsystems.clear();

	return ReturnCode;
}

SConsoleModuleSubsystem* SConsoleModule::GetSubsystem(SType* SubsystemClass)
{
	auto It = CachedSubsystemView.find(SubsystemClass->GetHashCode());
	if (It == CachedSubsystemView.end())
	{
		for (auto& Subsystem : Subsystems)
		{
			if (Subsystem->GetType()->IsDerivedFrom(SubsystemClass))
			{
				CachedSubsystemView.emplace(SubsystemClass->GetHashCode(), Subsystem);
				return Subsystem;
			}
		}
	}
	return It->second;
}