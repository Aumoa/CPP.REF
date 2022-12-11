// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/StackOverflowException.h"

StackOverflowException::StackOverflowException()
	: Exception(TEXT("The requested operation caused a stack overflow."))
{
}