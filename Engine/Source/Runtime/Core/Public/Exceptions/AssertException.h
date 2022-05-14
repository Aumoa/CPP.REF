// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace libty::inline Core
{
	class CORE_API AssertException : public Exception
	{
	public:
		AssertException(String exp);
		AssertException(String exp, String msg);
	};
}