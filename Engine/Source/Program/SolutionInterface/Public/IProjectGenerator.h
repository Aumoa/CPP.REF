// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectBuildRuntime.h"

interface IProject;
interface ISolution;

interface IProjectGenerator : implements SObject
{
	virtual IProject* GenerateProject(const ProjectBuildRuntime& RuntimeData) = 0;
	virtual ISolution* GenerateSolution() = 0;
};