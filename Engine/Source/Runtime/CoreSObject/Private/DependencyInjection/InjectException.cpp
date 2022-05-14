// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DependencyInjection/InjectException.h"

using namespace ::libty;

InjectException::InjectException(EInjectionErrorCode code, std::exception_ptr innerException)
	: Exception(FormatMessage(code), innerException)
{
}

String InjectException::FormatMessage(EInjectionErrorCode code)
{
	switch (code)
	{
	case EInjectionErrorCode::InjectFunctionNotFound:
		return TEXT("'Inject' function does not found in desired type.");
	case EInjectionErrorCode::InjectInnerException:
		return TEXT("Exception detected in 'Inject' function. See inner exception.");
	case EInjectionErrorCode::NotSupportedType:
		return TEXT("Object type is not allowed for desired service type.");
	case EInjectionErrorCode::ServiceNotFound:
		return TEXT("Service does not found in this provider.");
	case EInjectionErrorCode::ServiceNotConstructible:
		return TEXT("Service does not constructible without any parameters.");
	default:
		return code.ToString();
	}
}