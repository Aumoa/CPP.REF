// Copyright 2017 CLOVERGAMES Co., Ltd. All Rights Reserved.

#pragma once

#include "Exception.h"

class CORE_API InvalidCastException : public Exception
{
public:
	InvalidCastException(const String& message, std::exception_ptr innerException = nullptr);
	InvalidCastException(std::exception_ptr innerException = nullptr);
};