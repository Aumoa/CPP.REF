// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.System:ArgumentException;

export import :Exception;
export import :String;

export class CORE_API ArgumentException : public Exception
{
public:
	ArgumentException(String InArgumentName)
		: Exception(String::Format(TEXT("Invalid argument {} detected."), InArgumentName))
	{
	}
};