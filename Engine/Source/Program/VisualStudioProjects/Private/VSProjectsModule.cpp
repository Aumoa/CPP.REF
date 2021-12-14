// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "VSProjectsModule.h"
#include "Solution.h"
#include "VSProjectGenerator.h"
#include "LogVSProjects.h"

GENERATE_BODY(SVSProjectsModule);

int32 SVSProjectsModule::Run(const SCommandLine& CommandArgs)
{
	SE_LOG(LogVSProjects, Verbose, L"BuildTool Arguments: {}", StringUtils::Join(L", ", CommandArgs.GetArguments()));

	size_t Idx = CommandArgs.GetArgument(L"--Solution");
	if (Idx == -1)
	{
		FatalAndDisplay(L"--Solution argument required.");
		return -1;
	}

	auto* Solution = NewObject<SSolution>(*NewObject<SFileReference>(*CommandArgs.GetArgument(Idx + 1)));
	Solution->GenerateProjects(NewObject<SVSProjectGenerator>(Solution));

	LogAndDisplay(L"VisualStudio Project Generated.");
	return 0;
}

DEFINE_CONSOLE_MODULE(SVSProjectsModule);