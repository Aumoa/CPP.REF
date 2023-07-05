// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.System:NotImplementedException;

export import :Exception;
export import :String;

export class CORE_API NotImplementedException : public Exception
{
public:
	NotImplementedException()
		: Exception(TEXT("The function to call does not implemented."))
	{
	}
};