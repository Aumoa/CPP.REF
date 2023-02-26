// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NWindowsLaunch
{
	NWindowsLaunch() = delete;

public:
	static int32 Main(std::vector<String> Args);
};