// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ISolution.h"

class SVSProjectGenerator;

class SVSSolution : implements SObject, implements ISolution
{
	GENERATED_BODY(SVSSolution)

public:
	SVSSolution(SVSProjectGenerator* Generator);
};