// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class CompileErrorException : public Exception
	{
	public:
		CompileErrorException(String errorMessage) : Exception(std::move(errorMessage))
		{
		}

		virtual ~CompileErrorException() noexcept override
		{
		}
	};
}