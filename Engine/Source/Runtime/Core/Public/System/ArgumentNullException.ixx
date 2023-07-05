// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.System:ArgumentNullException;

export import :Exception;
export import :String;

export class CORE_API ArgumentNullException : public Exception
{
public:
	ArgumentNullException(String InArgumentName)
		: Exception(String::Format(TEXT("Argument {} cannot be nullptr."), InArgumentName))
	{
	}
};