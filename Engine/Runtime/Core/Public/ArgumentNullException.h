// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "ArgumentException.h"

class CORE_API ArgumentNullException : public ArgumentException
{
public:
	using Super = ArgumentException;

public:
	ArgumentNullException();
	ArgumentNullException(TRefPtr<String> message);
	ArgumentNullException(TRefPtr<String> message, Exception* innerException);
	~ArgumentNullException() override;
};