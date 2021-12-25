// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IProjectGenerator.h"
#include "VisualStudioVersion.h"

class SVSProjectGenerator : implements SObject, implements IProjectGenerator
{
	GENERATED_BODY(SVSProjectGenerator)
	friend class SVSSolution;

private:
	SPROPERTY(Solution)
	SSolution* Solution;
	EVisualStudioVersion VSVersion;

public:
	SVSProjectGenerator(SSolution* Solution, EVisualStudioVersion InVersion);

	virtual SSolution* GetSolution() override;
	virtual IProject* GenerateProject(const ProjectBuildRuntime& RuntimeData) override;
	virtual ISolution* GenerateSolution() override;

	EVisualStudioVersion GetVSVersion() { return VSVersion; }
};