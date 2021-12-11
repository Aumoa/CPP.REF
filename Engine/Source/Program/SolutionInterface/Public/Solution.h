// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IO/FileReference.h"
#include "ProjectBuildRuntime.h"

interface IProjectGenerator;
interface ISolution;

class SOLUTIONINTERFACE_API SSolution : implements SObject
{
	GENERATED_BODY(SSolution)

private:	
	std::wstring SolutionName;
	std::filesystem::path XmlBaseDirectory;
	std::filesystem::path GameRoot;
	std::filesystem::path EngineRoot;
	std::filesystem::path ThirdpartyRoot;
	std::wstring FirstProject;

	std::vector<ProjectBuildMetadata> ProjectMetadatas;
	std::map<std::wstring, ProjectBuildRuntime> ProjectRuntimes;

	Guid SolutionGuid;

public:
	SSolution(const SFileReference& SolutionXml);

	std::wstring GetSolutionName();
	std::wstring GetFirstProjectName();
	const std::map<std::wstring, ProjectBuildRuntime>& GetProjectRuntimes();
	ISolution* GenerateProjects(IProjectGenerator* Generator);

	const Guid& GetSolutionGuid();

private:
	void ReadSolutionXml(const SFileReference& SolutionXml);
	void SearchProjects(const std::filesystem::path& Directory, bool bIsEngine);
	bool TryParseProject(const std::filesystem::path& XmlPath, ProjectBuildMetadata& OutBuild);
	void GenerateProjectsRuntimeData();
	void BuildRuntime(ProjectBuildRuntime* Runtime);
	void GenerateProject(IProjectGenerator* Generator, ProjectBuildRuntime* Runtime);
};