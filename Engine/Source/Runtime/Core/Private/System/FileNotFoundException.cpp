// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:FileNotFoundException;

export import :Exception;

export class CORE_API FileNotFoundException : public Exception
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