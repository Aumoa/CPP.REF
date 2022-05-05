// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FatalException.h"

namespace libty::inline Core
{
	class CORE_API TaskCanceledException : public FatalException
	{
	public:
		TaskCanceledException(std::exception_ptr innerException = nullptr);
	};
}