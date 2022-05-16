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
		static void Write(ConsolePin pin, String message);
		static void Clear(ConsolePin pin = ConsolePin::Null());

		template<class... TArgs>
		static inline void Write(ConsolePin pin, String format, TArgs&&... args)
		{
			Write(pin, String::Format(format, std::forward<TArgs>(args)...));
		}

		template<class... TArgs>
		static inline void Write(String format, TArgs&&... args)
		{
			Write(ConsolePin::Null(), String::Format(format, std::forward<TArgs>(args)...));
		}

		template<class... TArgs>
		static inline void WriteLine(String format, TArgs&&... args)
		{
			Write(String::Format(format + TEXT("\n"), std::forward<TArgs>(args)...));
		}

		static void EnableLogToConsole(bool enabled);

	private:
		static void _Console_logged(const LogEntry& entry);
	};
}