// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

enum class EClassMetadata
{
	None,
	Abstract = 0x1
};

GENERATE_BITMASK_ENUM_OPERATORS(EClassMetadata);