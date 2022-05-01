// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "ConsoleColor.h"
#include "ConsolePin.h"

namespace libty::inline Console
{
	class InternalConsole
	{
	public:
		using BackForeColor = std::pair<EConsoleColor, EConsoleColor>;
		using Coord = std::pair<int32, int32>;

	public:
		static void SetColor(BackForeColor color) noexcept;
		static BackForeColor GetColor() noexcept;
		static void SetCursorPos(Coord coord) noexcept;
		static Coord GetCursorPos() noexcept;
		static int32 GetBufferWidth() noexcept;
		static void Clear() noexcept;

	public:
		static void Write(ConsolePin pin, std::wstring message) noexcept;
		static void Clear(ConsolePin pin, bool restore) noexcept;
	};
}