// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FatalException.h"

namespace libty::inline Core
{
	class CORE_API ArgumentNullException : public FatalException
	{
	public:
		ArgumentNullException(StringView argName);
	};
}