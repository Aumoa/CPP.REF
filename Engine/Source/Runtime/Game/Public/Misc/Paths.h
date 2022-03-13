// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <filesystem>

class GAME_API Paths
{
	SENUM(ERoot, int32,
		Game,,
		Engine,
	);

public:
	Paths() = delete;

public:
	static std::filesystem::path AsPhysicalPath(std::filesystem::path InLogicalPath);

private:
	static std::filesystem::path GetPhysicalRoot(ERoot Root);
};