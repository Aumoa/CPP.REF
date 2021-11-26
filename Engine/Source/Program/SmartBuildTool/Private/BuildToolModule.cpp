// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "BuildToolModule.h"
#include "BuildTool.h"

int32 SBuildToolModule::Main(const SCommandLine& CommandArgs)
{
	auto* Tool = NewObject<SBuildTool>();
	return Tool->Run(CommandArgs);
}

DEFINE_CONSOLE_MODULE(SBuildToolModule);