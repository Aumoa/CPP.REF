// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Exception.h"
#include "Diagnostics/StackTrace.h"

namespace Ayla
{
	Exception::Exception(const String& InMessage, std::exception_ptr InInnerException, std::source_location Src) noexcept
		: m_Message(InMessage)
		, m_InnerException(InInnerException)
		, m_Src(Src)
		, m_Stacktrace(StackTrace::Current().ToString())
	{
	}

	Exception::~Exception() noexcept
	{
	}

	String Exception::ToString() const noexcept
	{
		if (m_InnerException)
		{
			String Composed = String::Format(TEXT("{}: {}\n"), String::FromCodepage(typeid(*this).name()), m_Message);
			try
			{
				std::rethrow_exception(m_InnerException);
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
			Composed += String::Format(TEXT("--- End of inner exception stack trace ---\n{} in "), m_Stacktrace);
			return Composed;
		}
		else
		{
			return String::Format(TEXT("{}: {}\n{}"), String::FromCodepage(typeid(*this).name()), m_Message, m_Stacktrace);
		}
	}
}