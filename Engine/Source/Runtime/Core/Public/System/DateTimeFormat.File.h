// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/String.h"
#include "System/StaticClass.h"

class DateTime;

namespace DateTimeFormat
{
	struct CORE_API File : public StaticClass
	{
		static String ToString(const DateTime& Time);
	};
}