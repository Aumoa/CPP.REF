// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ConsoleModule.h"
#include "ConsoleModuleSubsystem.h"

using namespace ::libty;

DEFINE_LOG_CATEGORY(LogConsole);

SConsoleModule::SConsoleModule() : Super()
{
}

SConsoleModule::~SConsoleModule() noexcept
{
}

int32 SConsoleModule::Main(const CommandLine& commandLine)
{
	SharedPtr builder = SObjectFactory::CreateBuilder();
	Configure(builder.Get());

	_sp = Cast<SObjectFactory>(builder->Build());
	_sp->StartAsync().GetResult();
	_sp->GetServiceTask().GetResult();

	return 0;
}