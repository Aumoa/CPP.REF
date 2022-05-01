// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "ConsoleColor.h"
#include "ConsolePin.h"

namespace libty::inline Console
{
	class ConsolePin;

	class CONSOLE_API ConsoleEx
	{
	public:
		static void Write(ConsolePin pin, std::wstring message);
		static void Clear(ConsolePin pin = ConsolePin::Null());

		static inline void Write(ConsolePin pin, std::string message)
		{
			Write(pin, String::AsUnicode(message));
		}

		template<class TString, class... TArgs>
		static inline void Write(ConsolePin pin, TString&& format, TArgs&&... args) requires
			IString<TString, StringChar_t<TString>>
		{
			Write(pin, String::Format(format, std::forward<TArgs>(args)...));
		}

		template<class TString, class... TArgs>
		static inline void Write(TString&& format, TArgs&&... args) requires
			IString<TString, StringChar_t<TString>>
		{
			Write(ConsolePin::Null(), String::Format(format, std::forward<TArgs>(args)...));
		}

		template<class TString, class... TArgs>
		static inline void WriteLine(TString&& format, TArgs&&... args) requires
			IString<TString, StringChar_t<TString>>
		{
			using Char_t = StringChar_t<TString>;
			static Char_t nl[2] = { (Char_t)'\n', 0};
			static std::basic_string<Char_t> nls(nl);
			Write(std::forward<TString>(format) + nls, std::forward<TArgs>(args)...);
		}
	};
}