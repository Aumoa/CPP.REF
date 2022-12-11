// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exceptions/Exception.h"

class CORE_API CxxException : public Exception
{
public:
	CxxException(std::exception_ptr ptr);
};