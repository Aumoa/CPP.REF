// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.System:InvalidCastException;

export import :String;
export import :Exception;

export class CORE_API InvalidCastException : public Exception
{
public:
	InvalidCastException(String InMessage, std::exception_ptr InInnerException)
		: Exception(InMessage, InInnerException)
	{
	}
};