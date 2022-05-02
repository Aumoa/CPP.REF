// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "ConsoleModifiers.h"

namespace libty::inline Console
{
	struct ConsoleKeyInfo
	{
		EKey Key;
		wchar_t KeyChar;
		EConsoleModifiers Modifiers;

		static ConsoleKeyInfo Create(wchar_t keyChar, EKey key, bool shift, bool alt, bool control)
		{
			return ConsoleKeyInfo
			{
				.Key = key,
				.KeyChar = keyChar,
				.Modifiers =
					(shift ? EConsoleModifiers::Shift : EConsoleModifiers::None) |
					(alt ? EConsoleModifiers::Alt : EConsoleModifiers::None) |
					(control ? EConsoleModifiers::Control : EConsoleModifiers::None)
			};
		}
	};
}