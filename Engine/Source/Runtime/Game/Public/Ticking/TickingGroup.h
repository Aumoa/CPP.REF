// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

SENUM(ETickingGroup, int32,
	PrePhysics, 0,
	DuringPhysics, 1,
	PostPhysics, 2,
	PostUpdateWork, 3,
	NumGroups,
);