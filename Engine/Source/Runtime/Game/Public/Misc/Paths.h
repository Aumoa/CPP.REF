// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <filesystem>

class GAME_API Paths : public AbstractClass
{
	enum class ERoot
	{
		Game,
		Engine
	};

public:
	static std::filesystem::path AsPhysicalPath(std::filesystem::path InLogicalPath);

private:
	static std::filesystem::path GetPhysicalRoot(ERoot Root);
};