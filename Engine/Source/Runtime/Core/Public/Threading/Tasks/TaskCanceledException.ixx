// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.Threading:TaskCanceledException;

export import Core.Std;
export import Core.System;

export class CORE_API TaskCanceledException : public Exception
{
public:
	TaskCanceledException(std::exception_ptr InInnerException = nullptr, std::source_location Src = std::source_location::current())
		: Exception(TEXT("Task was aborted."), InInnerException, Src)
	{
	}
};