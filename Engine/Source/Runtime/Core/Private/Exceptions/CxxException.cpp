// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/CxxException.h"

CxxException::CxxException(std::exception_ptr ptr)
	: Exception(TEXT("Unmanaged CXX exception thrown."), ptr)
{
}