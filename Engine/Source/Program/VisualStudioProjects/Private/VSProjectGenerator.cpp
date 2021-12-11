// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "VSProjectGenerator.h"
#include "VSProject.h"
#include "VSSolution.h"

GENERATE_BODY(SVSProjectGenerator);

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