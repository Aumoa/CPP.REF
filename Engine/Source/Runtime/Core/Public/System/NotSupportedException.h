// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "System/Exception.h"

class CORE_API NotSupportedException : public Exception
{
public:
	NotSupportedException(std::exception_ptr InInnerException = nullptr);
	virtual ~NotSupportedException() noexcept override;
};