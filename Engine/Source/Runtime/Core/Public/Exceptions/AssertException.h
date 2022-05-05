// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FatalException.h"

namespace libty::inline Core
{
	class CORE_API AssertException : public FatalException
	{
	public:
		AssertException(StringView exp);
		AssertException(StringView exp, StringView msg);
	};
}