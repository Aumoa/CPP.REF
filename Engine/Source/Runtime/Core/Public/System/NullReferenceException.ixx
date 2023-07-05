// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.System:NullReferenceException;

export import :Exception;
export import :String;

export class CORE_API NullReferenceException : public Exception
{
public:
	NullReferenceException()
		: Exception(TEXT("Object reference not set to an instance of an object."))
	{
	}
};