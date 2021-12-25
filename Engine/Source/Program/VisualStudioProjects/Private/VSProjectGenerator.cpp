// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "VSProjectGenerator.h"
#include "VSProject.h"
#include "VSSolution.h"

GENERATE_BODY(SVSProjectGenerator);

SVSProjectGenerator::SVSProjectGenerator(SSolution* Solution, EVisualStudioVersion InVersion)
	: Super()
	, Solution(Solution)
	, VSVersion(InVersion)
{
}

SSolution* SVSProjectGenerator::GetSolution()
{
	return Solution;
}

IProject* SVSProjectGenerator::GenerateProject(const ProjectBuildRuntime& RuntimeData)
{
	return gcnew SVSProject(this, RuntimeData);
}

ISolution* SVSProjectGenerator::GenerateSolution()
{
	return gcnew SVSSolution(this);
}