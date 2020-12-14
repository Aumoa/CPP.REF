// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Exception.h"

namespace SC::Runtime::Core
{
	class CORE_API FormatException : public Exception
	{
	public:
		using Super = Exception;
		using This = FormatException;

	public:
		FormatException();
		FormatException(TRefPtr<String> message);
		FormatException(TRefPtr<String> message, Exception* innerException);
		~FormatException() override;
	};
}