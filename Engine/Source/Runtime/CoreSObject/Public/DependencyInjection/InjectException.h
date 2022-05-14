// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "InjectionErrorCode.h"

namespace libty::inline Core
{
	class CORESOBJECT_API InjectException : public Exception
	{
	public:
		InjectException(EInjectionErrorCode code, std::exception_ptr innerException = nullptr);

	private:
		static String FormatMessage(EInjectionErrorCode code);
	};
}