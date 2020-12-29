// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Exception.h"

class CORE_API InvalidArgumentException : public Exception
{
public:
	using Super = Exception;
	using This = InvalidArgumentException;

public:
	InvalidArgumentException();
	InvalidArgumentException(TRefPtr<String> message);
	InvalidArgumentException(TRefPtr<String> message, Exception* innerException);
	~InvalidArgumentException() override;
};