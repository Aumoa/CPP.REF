// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.System:NotSupportedException;

export import :Exception;
export import :String;

export class CORE_API NotSupportedException : public Exception
{
public:
	NotSupportedException(std::exception_ptr InInnerException = nullptr)
		: Exception(TEXT("The specified function or operation is not supported."), InInnerException)
	{
	}
};