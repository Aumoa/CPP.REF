// Copyright 2020-2022 Aumoa.lib. All right reserved.

import Core.System;
import Core.Diagnostics;

Exception::Exception(const String& InMessage, std::exception_ptr InInnerException, std::source_location Src) noexcept
	: Message(InMessage)
	, InnerException(InInnerException)
	, Src(Src)
	, Stacktrace(StackTrace::Current().ToString())
{
}

Exception::~Exception() noexcept
{
}

String Exception::ToString() const noexcept
{
	if (InnerException)
	{
		String Composed = String::Format(TEXT("{}: {}\n"), String(typeid(*this).name()), Message);
		try
		{
			std::rethrow_exception(InnerException);
		}
		catch (const Exception& E)
		{
			Composed += String::Format(TEXT("---> {}\n"), E.ToString());
		}
		catch (const std::exception& E)
		{
			Composed += String::Format(TEXT("---> {}: {}\n"), String(typeid(E).name()), String(E.what()));
		}
		catch (...)
		{
			Composed += String::Format(TEXT("---> Unknown exception.\n"));
		}
		Composed += String::Format(TEXT("--- End of inner exception stack trace ---\n{} in "), Stacktrace);
		return Composed;
	}
	else
	{
		return String::Format(TEXT("{}: {}\n{}"), String(typeid(*this).name()), Message, Stacktrace);
	}
}

String Exception::GetMessage() const noexcept
{
	return Message;
}

std::exception_ptr Exception::GetInnerException() const noexcept
{
	return InnerException;
}

std::source_location Exception::GetSourceLocation() const noexcept
{
	return Src;
}

String Exception::GetStackTrace() const noexcept
{
	return Stacktrace;
}