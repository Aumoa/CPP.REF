// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IProject.h"

interface IProjectGenerator;

class SVSProject : implements SObject, implements IProject
{
	GENERATED_BODY(SVSProject)

private:
	static std::string GetTypeString(ProjectBuildMetadata::EType Type);
	template<class T>
	static std::string_view BoolStr(T Expr);
	static std::string GetSubSystemString(ProjectBuildMetadata::EType Type);

	std::filesystem::path XmlPath;

public:
	SVSProject(IProjectGenerator* Generator, const ProjectBuildRuntime& RuntimeData);

	virtual std::filesystem::path GetPath() override;
};