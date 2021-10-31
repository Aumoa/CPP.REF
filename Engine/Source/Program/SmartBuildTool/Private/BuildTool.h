// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectBuildItem.h"

DECLARE_LOG_CATEGORY(, LogBuildTool);

class SBuildTool : implements SObject
{
	GENERATED_BODY(SBuildTool)

public:
	SBuildTool();

	int32 GuardedMain(std::vector<std::wstring_view> InArgs);

private:
	struct ProjectBuildFile
	{
		std::filesystem::path XmlPath;
		std::wstring Guid;
	};

	struct ProjectBuildRuntime
	{
		ProjectBuildMetadata* Metadata = nullptr;
		bool bBuild = false;

		ProjectBuildFile XmlFile;
		bool bXmlFileGenerated = false;

		std::wstring ProjectPath;

		std::set<ProjectBuildRuntime*> PublicReferences;
		std::set<ProjectBuildRuntime*> PrivateReferences;

		std::set<std::wstring> PublicIncludePaths;
		std::set<std::wstring> PrivateIncludePaths;
		std::wstring AdditionalIncludeDirectories;

		std::wstring PreprocessorDefinitions;

		std::set<int32> PublicDisableWarnings;
		std::set<int32> PrivateDisableWarnings;
		std::wstring DisableSpecificWarnings;

		std::set<std::wstring> PublicExternalLinks;
		std::set<std::wstring> PrivateExternalLinks;
		std::wstring AdditionalDependencies;

		// While solution build.
		std::wstring CachedSlicedSolutionDirectory;
	};

private:
	std::wstring SolutionName;
	std::vector<ProjectBuildMetadata> ProjectsMetadata;
	std::map<std::wstring, ProjectBuildRuntime> ProjectsRuntime;

private:
	int32 SearchProjects(const std::filesystem::path& DirectoryPath);
	bool TryParseProject(const std::filesystem::path& XmlPath, ProjectBuildMetadata& OutBuild);

	int32 GenerateProjectsRuntimeData();
	void BuildRuntime(ProjectBuildRuntime* Runtime);

	int32 GenerateProjectFiles();
	int32 GenerateProjectFile(ProjectBuildRuntime* Runtime);

	int32 GenerateSolutionFile();

	static EType ParseType(const char* StringToken);
	static EAccessKey ParseAccessKey(const char* StringToken);
	static std::string GetTypeString(EType Type);
	static std::string GetSubSystemString(EType Type);
	static std::wstring NewGuid();
};