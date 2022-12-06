// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/NullReferenceException.h"

NullReferenceException::NullReferenceException()
	: Exception(TEXT("Object reference not set to an instance of an object."))
{
}