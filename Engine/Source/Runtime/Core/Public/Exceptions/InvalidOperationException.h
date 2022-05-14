// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace libty::inline Core
{
	class CORE_API InvalidOperationException : public Exception
	{
	public:
		InvalidOperationException(String message, std::exception_ptr innerException = nullptr);
		InvalidOperationException(std::exception_ptr innerException = nullptr);
	};
}