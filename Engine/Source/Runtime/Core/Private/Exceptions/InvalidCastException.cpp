// Copyright 2017 CLOVERGAMES Co., Ltd. All Rights Reserved.

#include "Exceptions/InvalidCastException.h"

InvalidCastException::InvalidCastException(const String& message, std::exception_ptr innerException)
	: Exception(message, innerException)
{
}

InvalidCastException::InvalidCastException(std::exception_ptr innerException)
	: InvalidCastException(TEXT("Cannot cast from source type to destination type."), innerException)
{
}