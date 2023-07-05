// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.System:StackOverflowException;

export import :Exception;
export import :String;

export class CORE_API StackOverflowException : public Exception
{
public:
	StackOverflowException()
		: Exception(TEXT("The requested operation caused a stack overflow."))
	{
	}
};