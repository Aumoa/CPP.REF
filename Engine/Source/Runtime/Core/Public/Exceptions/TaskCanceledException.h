// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

class CORE_API TaskCanceledException : public Exception
{
public:
	TaskCanceledException(std::exception_ptr innerException = nullptr);
};