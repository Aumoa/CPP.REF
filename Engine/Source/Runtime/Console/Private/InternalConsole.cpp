// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "InternalConsole.h"
#include <stack>

#if PLATFORM_WINDOWS

#include <Windows.h>
#undef Yield

namespace libty::inline Console
{
	static HANDLE sCachedStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	void InternalConsole::SetColor(BackForeColor color) noexcept
	{
		SetConsoleTextAttribute(sCachedStdout, ((WORD)color.first << 8) | (BYTE)color.second);
	}

	auto InternalConsole::GetColor() noexcept -> BackForeColor
	{
		CONSOLE_SCREEN_BUFFER_INFO screen;
		GetConsoleScreenBufferInfo(sCachedStdout, &screen);
		return { (EConsoleColor)((screen.wAttributes >> 8) & 0xFF), (EConsoleColor)(screen.wAttributes & 0xFF) };
	}

	void InternalConsole::SetCursorPos(Coord coord) noexcept
	{
		SetConsoleCursorPosition(sCachedStdout, { (short)coord.first, (short)coord.second });
	}

	std::pair<int32, int32> InternalConsole::GetCursorPos() noexcept
	{
		CONSOLE_SCREEN_BUFFER_INFO screen;
		GetConsoleScreenBufferInfo(sCachedStdout, &screen);
		return { (int32)screen.dwCursorPosition.X, (int32)screen.dwCursorPosition.Y };
	}

	int32 InternalConsole::GetBufferWidth() noexcept
	{
		CONSOLE_SCREEN_BUFFER_INFO screen;
		GetConsoleScreenBufferInfo(sCachedStdout, &screen);
		return (int32)screen.dwSize.X;
	}

	void InternalConsole::Clear() noexcept
	{
		std::system("cls");
	}
}

#endif

namespace libty::inline Console
{
	struct ScriptStack
	{
		std::stack<EConsoleColor> Backgrounds;
		std::stack<EConsoleColor> Foregrounds;

		static ScriptStack Current()
		{
			auto [back, fore] = InternalConsole::GetColor();
			ScriptStack ss;
			ss.Backgrounds.emplace(back);
			ss.Foregrounds.emplace(fore);
			return ss;
		}

		void Release()
		{
			while (!ensure(Backgrounds.size() <= 1))
			{
				Backgrounds.pop();
			}

			while (!ensureMsgf(Foregrounds.size() <= 1, L"<color=...> script is not released with {0} stacks.", Foregrounds.size()))
			{
				Foregrounds.pop();
			}
		}
	};

	void InternalConsole::Write(ConsolePin pin, String message) noexcept
	{
		InternalConsole::Coord restore;
		if (pin.IsValid())
		{
			restore = InternalConsole::GetCursorPos();
			InternalConsole::SetCursorPos({ 0, pin._impl->_y });
		}

		ScriptStack ss = ScriptStack::Current();
		size_t pos = 0;
		size_t prev = 0;
		while ((pos = message.IndexOf(TEXT('<'), pos)) != -1)
		{
			if (pos != prev)
			{
				size_t len = pos - prev;
				std::wcout << message.SubstringView(prev, len);
			}

			if (message.length() > pos + 1 && message[pos + 1] == L'<')
			{
				std::wcout << L'<';
				pos += 2;
				prev = pos;
				continue;
			}

			size_t endof = message.find(L'>', pos);
			std::vector<std::wstring> code = String::Split(message.substr(pos + 1, endof - (pos + 1)), L"=", true, true);
			if (String::Equals(code[0], L"color", true) && code.size() == 2)
			{
				EConsoleColor foreground;
				if (!Reflection::SEnum::TryParse<EConsoleColor>(code[1], foreground))
				{
					foreground = ss.Foregrounds.top();
				}

				EConsoleColor background = ss.Backgrounds.top();
				ss.Foregrounds.emplace(foreground);
				InternalConsole::SetColor({ background, foreground });
			}
			else if (String::Equals(code[0], L"/color", true) && ss.Foregrounds.size() > 1)
			{
				ss.Foregrounds.pop();
				EConsoleColor foreground = ss.Foregrounds.top();
				EConsoleColor background = ss.Backgrounds.top();
				InternalConsole::SetColor({ background, foreground });
			}

			pos = endof + 1;
			prev = pos;
		}

		ss.Release();
		InternalConsole::SetColor({ ss.Backgrounds.top(), ss.Foregrounds.top() });

		pos = message.length();
		if (prev < pos)
		{
			if (prev == 0)
			{
				std::wcout << message;
			}
			else
			{
				std::wcout << message.substr(prev, pos - prev);
			}
		}

		if (pin.IsValid())
		{
			InternalConsole::SetCursorPos(restore);
		}
	}

	void InternalConsole::Clear(ConsolePin pin, bool restore) noexcept
	{
		if (pin.IsValid())
		{
			InternalConsole::Coord restorePos;
			if (restore)
			{
				restorePos = InternalConsole::GetCursorPos();
			}

			std::string pad = std::string((size_t)GetBufferWidth(), ' ');
			for (int32 i = 0; i < pin._impl->_height; ++i)
			{
				std::cout << pad << std::endl;
			}

			if (restore)
			{
				InternalConsole::SetCursorPos(restorePos);
			}
		}
		else
		{
			Clear();
		}
	}
}