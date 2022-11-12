// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exceptions/Exception.h"

class CORE_API SystemException : public Exception
{
public:
	SystemException(int32 systemCode) noexcept;
	SystemException(int32 systemCode, String message) noexcept;

private:
	static String FormatMessage(int32 systemCode, String message = TEXT("")) noexcept;
};