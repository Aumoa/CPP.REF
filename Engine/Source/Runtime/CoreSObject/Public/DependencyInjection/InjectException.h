// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "InjectionErrorCode.h"

namespace libty::inline Core
{
	class CORESOBJECT_API InjectException : public FatalException
	{
	public:
		InjectException(EInjectionErrorCode code, std::exception_ptr innerException = nullptr, std::source_location src = std::source_location::current());

	private:
		static std::string FormatMessage(EInjectionErrorCode code);
	};
}