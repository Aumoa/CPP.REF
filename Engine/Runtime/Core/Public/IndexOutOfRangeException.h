// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Exception.h"

class CORE_API IndexOutOfRangeException : public Exception
{
public:
	using Super = Exception;
	using This = IndexOutOfRangeException;

public:
	IndexOutOfRangeException();
	IndexOutOfRangeException(TRefPtr<String> message);
	IndexOutOfRangeException(TRefPtr<String> message, Exception* innerException);
	~IndexOutOfRangeException() override;
};