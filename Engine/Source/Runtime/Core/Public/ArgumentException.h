// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"
#include <source_location>

namespace Ayla
{
	class CORE_API ArgumentException : public Exception
	{
	public:
		ArgumentException(String InArgumentName)
			: Exception(String::Format(TEXT("Invalid argument {} detected."), InArgumentName))
		{
		}

		static void ThrowIf(bool expression, String message, std::source_location src = std::source_location::current())
		{
			if (!expression)
			{
				throw ArgumentException(message + String::Format(TEXT(" at {}"), FormatSourceLocation(src)));
			}
		}

	private:
		static String FormatSourceLocation(std::source_location src)
		{
			return String::Format(TEXT("{}:{} in function {}"), String::FromLiteral(src.file_name()), src.line(), String::FromLiteral(src.function_name()));
		}
	};
}