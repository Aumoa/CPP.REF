// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace libty::inline Core
{
	class CORE_API AssertException : public Exception
	{
	public:
		AssertException(StringView exp);
		AssertException(StringView exp, StringView msg);
	};
}