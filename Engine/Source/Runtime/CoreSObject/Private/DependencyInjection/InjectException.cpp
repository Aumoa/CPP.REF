// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DependencyInjection/InjectException.h"

using namespace ::libty;

InjectException::InjectException(EInjectionErrorCode code, std::exception_ptr innerException)
	: FatalException(FormatMessage(code), innerException)
{
}

std::string InjectException::FormatMessage(EInjectionErrorCode code)
{
	switch (code)
	{
	case EInjectionErrorCode::InjectFunctionNotFound:
		return "'Inject' function does not found in desired type.";
	case EInjectionErrorCode::InjectInnerException:
		return "Exception detected in 'Inject' function. See inner exception.";
	case EInjectionErrorCode::NotSupportedType:
		return "Object type is not allowed for desired service type.";
	case EInjectionErrorCode::ServiceNotFound:
		return "Service does not found in this provider.";
	case EInjectionErrorCode::ServiceNotConstructible:
		return "Service does not constructible without any parameters.";
	default:
		return String::AsMultibyte(code.ToString());
	}
}