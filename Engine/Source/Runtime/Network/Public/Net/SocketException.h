// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Exceptions/Exception.h"

class NETWORK_API SocketException : public Exception
{
private:
	using Super = Exception;

public:
	SocketException(const String& message, std::exception_ptr innerException = nullptr);
};