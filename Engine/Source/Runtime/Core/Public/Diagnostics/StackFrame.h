// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "System/String.h"

struct CORE_API StackFrame
{
	String Location;
	int32 Line = 0;
	String Module;
	String Description;
	int64 AddressOf = 0;
};