// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Exceptions/Exception.h"

class CORE_API SInvalidArgumentException : public SException
{
	GENERATED_BODY(SInvalidArgumentException)

public:
	SInvalidArgumentException();
	SInvalidArgumentException(std::wstring_view Message, SException* InnerException = nullptr);
};