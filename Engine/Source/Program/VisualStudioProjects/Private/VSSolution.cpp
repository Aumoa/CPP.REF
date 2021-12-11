// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "VSSolution.h"
#include "VSProjectGenerator.h"
#include "Solution.h"
#include "IProject.h"
#include "GuidHelper.h"
#include "Misc/CrcHash.h"

GENERATE_BODY(SVSSolution);

SVSSolution::SVSSolution(SVSProjectGenerator* Generator)
{
	SSolution* Solution = Generator->Solution;

	std::wstringstream Builder;

	constexpr std::wstring_view CppProjectGuid = L"8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942";
	constexpr std::wstring_view SolutionDirectoryGuid = L"2150E333-8FDC-42A3-9474-1A3956D46DE8";

	// Generate header.
	Builder << std::endl;
	Builder << L"Microsoft Visual Studio Solution File, Format Version 12.00" << std::endl;
	Builder << L"# Visual Studio Version 16" << std::endl;
	Builder << L"VisualStudioVersion = 16.0.31321.278" << std::endl;
	Builder << L"MinimumVisualStudioVersion = 10.0.40219.1" << std::endl;

	// Generate solution directory.
	struct SolutionDirectoryPair
	{
		std::wstring RecursivePath;
		std::wstring Guid;
	};

	std::map<std::wstring, std::wstring> CachedSlicedSolutionDirectory;

	std::map<std::wstring, SolutionDirectoryPair> SolutionDirectories;
	for (auto& [Key, Value] : Solution->GetProjectRuntimes())
	{
		std::vector<std::wstring> RecursivePathSlice = StringUtils::Split(Value.Metadata->Path, L".", true, true);
		RecursivePathSlice.erase(RecursivePathSlice.end() - 1);
		RecursivePathSlice.insert(RecursivePathSlice.begin(), Value.Metadata->BaseDirectory);

		std::wstring RecursivePath;
		for (auto& Slice : RecursivePathSlice)
		{
			RecursivePath += Slice;

			auto [It, Status] = SolutionDirectories.emplace(Slice, SolutionDirectoryPair());
			if (Status)
			{
				It->second.RecursivePath = RecursivePath;
				It->second.Guid = GuidHelper::GenerateGuid(RecursivePath).ToString();
			}

			RecursivePath += L".";
		}

		CachedSlicedSolutionDirectory[Key] = RecursivePathSlice.back();
	}

	for (auto& [Key, Value] : SolutionDirectories)
	{
		Builder << std::format(LR"(Project("{{{0}}}") = "{1}", "{1}", "{{{2}}}")", SolutionDirectoryGuid, Key, Value.Guid) << std::endl;
		Builder << L"EndProject" << std::endl;
	}

	// Generate C++ projects.
	auto& ProjectRuntimes = Solution->GetProjectRuntimes();
	std::wstring FirstProjectName = Solution->GetFirstProjectName();

	std::vector<std::wstring> ProjectsSort;
	ProjectsSort.reserve(ProjectRuntimes.size());
	for (auto& [Key, Value] : ProjectRuntimes)
	{
		std::wstring ProjectDeclare = std::format(LR"(Project("{{{0}}}") = "{1}", "{2}", "{{{3}}}"
EndProject)", CppProjectGuid, Key, Value.GeneratedProject->GetPath().wstring(), Value.ProjectGuid.ToString());

		auto Where = Key == FirstProjectName ? ProjectsSort.begin() : ProjectsSort.end();
		ProjectsSort.emplace(Where, ProjectDeclare);
	}

	for (auto& Declare : ProjectsSort)
	{
		Builder << Declare << std::endl;
	}

	struct BuildConfiguration
	{
		std::wstring Name;
	};

	BuildConfiguration Configurations[] =
	{
		{
			.Name = L"Debug"
		},
		{
			.Name = L"Release"
		}
	};

	Builder << L"Global" << std::endl;
	{
		// SolutionConfigurationPlatforms
		Builder << L"\tGlobalSection(SolutionConfigurationPlatforms) = preSolution" << std::endl;
		{
			for (auto& Config : Configurations)
			{
				Builder << std::format(L"\t\t{0}|x64 = {0}|x64", Config.Name) << std::endl;
			}
		}
		Builder << L"\tEndGlobalSection" << std::endl;

		// ProjectConfigurationPlatforms
		Builder << L"\tGlobalSection(ProjectConfigurationPlatforms) = postSolution" << std::endl;
		{
			for (auto& [Key, Project] : ProjectRuntimes)
			{
				for (auto& Config : Configurations)
				{
					Builder << std::format(L"\t\t{{{0}}}.{1}|x64.ActiveCfg = {1}|x64", Project.ProjectGuid.ToString(), Config.Name) << std::endl;
					Builder << std::format(L"\t\t{{{0}}}.{1}|x64.Build.0 = {1}|x64", Project.ProjectGuid.ToString(), Config.Name) << std::endl;
				}
			}
		}
		Builder << L"\tEndGlobalSection" << std::endl;

		// SolutionProperties
		Builder << L"\tGlobalSection(SolutionProperties) = preSolution" << std::endl;
		{
			Builder << L"\t\tHideSolutionNode = FALSE" << std::endl;
		}
		Builder << L"\tEndGlobalSection" << std::endl;

		// NestedProjects
		Builder << L"\tGlobalSection(NestedProjects) = preSolution" << std::endl;
		{
			for (auto& [Key, Value] : SolutionDirectories)
			{
				std::vector<std::wstring> Splits = StringUtils::Split(Value.RecursivePath, L".");
				if (Splits.size() > 1)
				{
					std::wstring LastParent = Splits[Splits.size() - 2];
					SolutionDirectoryPair LastParentInfo = SolutionDirectories[LastParent];
					Builder << std::format(L"\t\t{{{}}} = {{{}}}", Value.Guid, LastParentInfo.Guid) << std::endl;
				}
			}

			for (auto& [Key, Project] : ProjectRuntimes)
			{
				SolutionDirectoryPair ParentInfo = SolutionDirectories[CachedSlicedSolutionDirectory[Key]];
				Builder << std::format(L"\t\t{{{}}} = {{{}}}", Project.ProjectGuid.ToString(), ParentInfo.Guid) << std::endl;
			}
		}
		Builder << L"\tEndGlobalSection" << std::endl;

		// ExtensibilityGlobals
		Builder << L"\tGlobalSection(ExtensibilityGlobals) = postSolution" << std::endl;
		{
			Builder << std::format(L"\t\tSolutionGuid = {{{}}}", Solution->GetSolutionGuid().ToString()) << std::endl;
		}
		Builder << L"\tEndGlobalSection" << std::endl;
	}
	Builder << L"EndGlobal" << std::endl;

	Builder << std::endl;

	std::filesystem::path SolutionFile = std::format(L"{}.sln", Solution->GetSolutionName());

	std::string Previous = StringUtils::Trim(NewObject<SFileReference>(SolutionFile)->ReadAllText());
	std::string Build = StringUtils::Trim(WCHAR_TO_ANSI(Builder.str()));
	if (Previous != Build)
	{
		NewObject<SFileReference>(SolutionFile)->WriteAllText(Build);
	}
}