// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exceptions/Exception.h"

class CORE_API SocketException : public Exception
{
public:
	SocketException(int32 systemCode) noexcept;
	SocketException(int32 systemCode, String message) noexcept;

private:
	static String FormatMessage(int32 systemCode, String message = TEXT("")) noexcept;
};