// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class GAME_API Paths abstract final
{
public:
	static std::filesystem::path GetContentPath();
};