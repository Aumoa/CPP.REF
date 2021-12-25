// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "VSProjectsModule.h"
#include "Solution.h"
#include "VSProjectGenerator.h"
#include "VisualStudioVersion.h"
#include "LogVSProjects.h"

GENERATE_BODY(SVSProjectsModule);

int32 SVSProjectsModule::Run(const SCommandLine& CommandArgs)
{
	SE_LOG(LogVSProjects, Verbose, L"BuildTool Arguments: {}", StringUtils::Join(L", ", CommandArgs.GetArguments()));

	size_t Idx = CommandArgs.GetArgument(L"--Solution");
	if (Idx == -1)
	{
		ErrorAndDisplay(L"--Solution argument required.");
		return -1;
	}

	std::wstring SolutionXml = std::wstring(*CommandArgs.GetArgument(Idx + 1));

	EVisualStudioVersion Version = EVisualStudioVersion::VS2019;
	Idx = CommandArgs.GetArgument(L"--VS");
	if (Idx != -1)
	{
		std::wstring VersionStr = std::wstring(CommandArgs.GetArgument(Idx + 1).value_or(L"Unknown"));
		if (VersionStr.find(L"2019") != std::wstring::npos)
		{
			Version = EVisualStudioVersion::VS2019;
		}
		else if (VersionStr.find(L"2022") != std::wstring::npos)
		{
			Version = EVisualStudioVersion::VS2022;
		}
		else
		{
			ErrorAndDisplay(L"Error: Invalid version string: {}. Use VS2019 instead.", VersionStr);
		}
	}

	auto* Solution = gcnew SSolution(*(gcnew SFileReference(SolutionXml)));
	Solution->GenerateProjects(gcnew SVSProjectGenerator(Solution, Version));

	LogAndDisplay(L"VisualStudio Project Generated.");
	return 0;
}

DEFINE_CONSOLE_MODULE(SVSProjectsModule);