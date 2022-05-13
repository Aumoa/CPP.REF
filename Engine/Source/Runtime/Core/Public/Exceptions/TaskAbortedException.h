// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace libty::inline Core
{
	class CORE_API TaskAbortedException : public Exception
	{
	public:
		TaskAbortedException(std::exception_ptr innerException = nullptr);
	};
}