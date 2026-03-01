// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Exception.h"
#include <sstream>

namespace Ayla
{
	Exception::Exception(const String& InMessage, std::exception_ptr InInnerException, std::source_location Src) noexcept
		: m_Message(InMessage)
		, m_InnerException(InInnerException)
		, m_Src(Src)
#if __has_include(<stacktrace>)
		, m_Stacktrace(std::stacktrace::current())
#endif
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

#if __has_include(<stacktrace>)
			auto ss = std::format("{}", m_Stacktrace);
			Composed += String::Format(TEXT("--- End of inner exception stack trace ---\n{} in "), String::FromCodepage(ss));
#else
			Composed += String::Format(TEXT("--- End of inner exception stack trace ---\n"));
#endif
			return Composed;
		}
		else
		{
#if __has_include(<stacktrace>)
			auto ss = std::format("{}", m_Stacktrace);
			return String::Format(TEXT("{}: {}\n{}"), String::FromCodepage(typeid(*this).name()), m_Message, String::FromCodepage(ss));
#else
			return String::Format(TEXT("{}: {}"), String::FromCodepage(typeid(*this).name()), m_Message);
#endif
		}
	}
}