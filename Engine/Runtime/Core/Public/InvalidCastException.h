// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Exception.h"

namespace SC::Runtime::Core
{
	class CORE_API InvalidCastException : public Exception
	{
	public:
		using Super = Exception;
		using This = InvalidCastException;

	public:
		InvalidCastException();
		InvalidCastException(TRefPtr<String> message);
		InvalidCastException(TRefPtr<String> message, Exception* innerException);
		~InvalidCastException() override;
	};
}