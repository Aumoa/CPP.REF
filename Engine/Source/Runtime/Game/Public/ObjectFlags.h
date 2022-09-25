// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectFlags.gen.h"

SENUM(Flags)
enum class EObjectFlags
{
	None			= 0,
	Active			= 0b00000001,
	BegunPlay		= 0b00000010,
	PendingKill		= 0b00000100,
	Registered		= 0b00001000,
};