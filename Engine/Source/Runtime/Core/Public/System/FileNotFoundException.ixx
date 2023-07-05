// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.System:FileNotFoundException;

export import :Exception;
export import :String;

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