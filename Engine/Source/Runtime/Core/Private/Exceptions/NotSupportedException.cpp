// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/NotSupportedException.h"

NotSupportedException::NotSupportedException(std::exception_ptr InnerException)
	: Exception(TEXT("The specified function or operation is not supported."), InnerException)
{
}

NotSupportedException::NotSupportedException(String InMessage, std::exception_ptr InnerException)
	: Exception(InMessage, InnerException)
{
}