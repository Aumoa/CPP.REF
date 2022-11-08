// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "Misc/String.h"
#include "Environment.gen.h"

SCLASS()
class CORE_API Environment : implements Object
{
	GENERATED_BODY()

public:
	static String GetCurrentDirectory();
};