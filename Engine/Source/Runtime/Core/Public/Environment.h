// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include "Misc/StaticClass.h"

class CORE_API Environment : public StaticClass
{
public:
	static String GetCurrentDirectory();
};