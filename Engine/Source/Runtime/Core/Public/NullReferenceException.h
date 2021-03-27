// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Exception.h"

class CORE_API NullReferenceException : public Exception
{
public:
	using Super = Exception;
	using This = NullReferenceException;

public:
	NullReferenceException();
	NullReferenceException(TRefPtr<String> message);
	NullReferenceException(TRefPtr<String> message, Exception* innerException);
	~NullReferenceException() override;
};