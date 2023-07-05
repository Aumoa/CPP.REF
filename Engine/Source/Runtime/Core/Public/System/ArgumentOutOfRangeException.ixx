// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.System:ArgumentOutOfRangeException;

export import :Exception;
export import :String;

export class CORE_API ArgumentOutOfRangeException : public Exception
{
public:
	ArgumentOutOfRangeException()
		: Exception(TEXT("Argument is out of range."))
	{
	}
};