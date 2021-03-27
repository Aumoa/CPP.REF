// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Exception.h"

class CORE_API ArgumentException : public Exception
{
public:
	using Super = Exception;

public:
	ArgumentException();
	ArgumentException(TRefPtr<String> message);
	ArgumentException(TRefPtr<String> message, Exception* innerException);
	~ArgumentException() override;
};