// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "VSProjectsModule.h"
#include "Solution.h"
#include "VSProjectGenerator.h"
#include "LogVSProjects.h"

int32 SVSProjectsModule::Main(const SCommandLine& CommandArgs)
{
	SE_LOG(LogVSProjects, Verbose, L"BuildTool Arguments: {}", StringUtils::Join(L", ", CommandArgs.GetArguments()));

	size_t Idx = CommandArgs.GetArgument(L"--Solution");
	if (Idx == -1)
	{
		SE_LOG(LogVSProjects, Fatal, L"--Solution argument required.");
		return -1;
	}

	auto* Solution = NewObject<SSolution>(SFileReference(*CommandArgs.GetArgument(Idx + 1)));
	Solution->GenerateProjects(NewObject<SVSProjectGenerator>(Solution));

	SE_LOG(LogVSProjects, Verbose, L"VisualStudio Project Generated.");
	return 0;
}

DEFINE_CONSOLE_MODULE(SVSProjectsModule);