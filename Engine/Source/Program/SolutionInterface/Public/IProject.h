// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectBuildRuntime.h"

interface IProject : implements SObject
{
	virtual std::filesystem::path GetPath() = 0;
};