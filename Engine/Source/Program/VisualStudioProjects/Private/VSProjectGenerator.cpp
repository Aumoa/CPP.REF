// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "VSProjectGenerator.h"
#include "VSProject.h"
#include "VSSolution.h"

SVSProjectGenerator::SVSProjectGenerator(SSolution* Solution)
	: Super()
	, Solution(Solution)
{
}

IProject* SVSProjectGenerator::GenerateProject(const ProjectBuildRuntime& RuntimeData)
{
	return NewObject<SVSProject>(this, RuntimeData);
}

ISolution* SVSProjectGenerator::GenerateSolution()
{
	return NewObject<SVSSolution>(this);
}