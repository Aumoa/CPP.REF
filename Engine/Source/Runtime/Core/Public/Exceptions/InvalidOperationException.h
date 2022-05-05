// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FatalException.h"

namespace libty::inline Core
{
	class CORE_API InvalidOperationException : public FatalException
	{
	public:
		InvalidOperationException(StringView message, std::exception_ptr innerException = nullptr);
		InvalidOperationException(std::exception_ptr innerException = nullptr);
	};
}