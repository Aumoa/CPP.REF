// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectBuildRuntime.h"

interface IProject;
interface ISolution;
class SSolution;

interface IProjectGenerator : implements SObject
{
	virtual SSolution* GetSolution() = 0;
	virtual IProject* GenerateProject(const ProjectBuildRuntime& RuntimeData) = 0;
	virtual ISolution* GenerateSolution() = 0;
};