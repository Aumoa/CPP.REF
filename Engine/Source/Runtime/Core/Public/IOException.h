// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Exception.h"

class CORE_API IOException : public Exception
{
public:
	using Super = Exception;

public:
	IOException();
	IOException(TRefPtr<String> message);
	IOException(TRefPtr<String> message, Exception* innerException);
	~IOException() override;
};