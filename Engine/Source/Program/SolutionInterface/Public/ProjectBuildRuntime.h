// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectBuildMetadata.h"
#include "Guid.h"

interface IProject;

struct ProjectBuildRuntime
{
	ProjectBuildMetadata* Metadata = nullptr;
	bool bBuild = false;

	std::wstring ProjectPath;
	Guid ProjectGuid;
	IProject* GeneratedProject = nullptr;

	std::map<Guid, ProjectBuildRuntime*> PublicReferences;
	std::map<Guid, ProjectBuildRuntime*> PrivateReferences;

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
};