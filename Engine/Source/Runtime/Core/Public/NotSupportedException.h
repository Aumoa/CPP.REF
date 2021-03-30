// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Exception.h"

class CORE_API NotSupportedException : public Exception
{
public:
	using Super = Exception;

public:
	NotSupportedException();
	NotSupportedException(TRefPtr<String> message);
	NotSupportedException(TRefPtr<String> message, Exception* innerException);
	~NotSupportedException() override;
};