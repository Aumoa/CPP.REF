// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

class CORE_API NotSupportedException : public Exception
{
public:
	NotSupportedException(std::exception_ptr InnerException = nullptr);
	NotSupportedException(String InMessage, std::exception_ptr InnerException = nullptr);
};