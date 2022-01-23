// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Exceptions/Exception.h"

class SOCKETS_API SSocketException : public SException
{
	GENERATED_BODY(SSocketException)

public:
	SSocketException(std::wstring_view Message, SException* InnerException = nullptr);
};