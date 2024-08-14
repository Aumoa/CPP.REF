// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "System/StaticClass.h"
#include "Platform/PlatformMacros.h"

struct CORE_API GenericPlatformIO : public StaticClass
{
	static constexpr size_t OVERLAPPED_SIZE = 1;
};