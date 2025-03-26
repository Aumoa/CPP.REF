// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "StaticClass.h"

namespace Ayla
{
	class DateTime;

	namespace DateTimeFormat
	{
		struct CORE_API Json : public StaticClass
		{
			static String ToString(const DateTime& Time);
		};
	}
}