// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

class CORE_API InvalidOperationException : public Exception
{
public:
	InvalidOperationException(const String& message, std::exception_ptr innerException = nullptr);
	InvalidOperationException(std::exception_ptr innerException = nullptr);
};