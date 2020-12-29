// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Exception.h"

class CORE_API ArgumentNullException : public Exception
{
public:
	using Super = Exception;
	using This = ArgumentNullException;

public:
	ArgumentNullException();
	ArgumentNullException(TRefPtr<String> message);
	ArgumentNullException(TRefPtr<String> message, Exception* innerException);
	~ArgumentNullException() override;
};