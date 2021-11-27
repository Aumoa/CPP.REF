// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IProjectGenerator.h"

class SSolution;

class SVSProjectGenerator : implements SObject, implements IProjectGenerator
{
	GENERATED_BODY(SVSProjectGenerator)
	friend class SVSSolution;

private:
	SSolution* Solution;

public:
	SVSProjectGenerator(SSolution* Solution);

	virtual IProject* GenerateProject(const ProjectBuildRuntime& RuntimeData) override;
	virtual ISolution* GenerateSolution() override;
};