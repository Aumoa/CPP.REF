// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace Ayla
{
	class CORE_API FileNotFoundException : public Exception
	{
	public:
		FileNotFoundException(String InFilename, std::exception_ptr InInnerException = nullptr)
			: Exception(FormatMessage(InFilename), InInnerException)
		{
		}

	private:
		static String FormatMessage(String InFilename)
		{
			if (InFilename.IsEmpty())
			{
				return TEXT("File not found.");
			}
			else
			{
				return String::Format(TEXT("Desired file '{}' is not found."), InFilename);
			}
		}
	};
}