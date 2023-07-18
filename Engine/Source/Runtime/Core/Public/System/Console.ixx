// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

export module Core.System:Console;

export import :String;
export import :ConsoleColor;

export class CORE_API Console
{
public:
	static class TextWriter& GetOut();
	static class TextWriter& GetError();

public:
	static void Write(String Str);
	static void WriteLine(String Str);

	static String ReadLine();

	static void SetForegroundColor(EConsoleColor InColor);
	static EConsoleColor GetForegroundColor();
	static void SetBackgroundColor(EConsoleColor InColor);
	static EConsoleColor GetBackgroundColor();
	static void SetColors(EConsoleColor InForegroundColor, EConsoleColor InBackgroundColor);
	static void GetColors(EConsoleColor* OutForegroundColor, EConsoleColor* OutBackgroundColor);

public:
	template<class... TArgs>
	static void Write(String Format, TArgs&&... InArgs)
	{
		Write(String::Format(Format, std::forward<TArgs>(InArgs)...));
	}

	template<class... TArgs>
	static void WriteLine(String Format, TArgs&&... InArgs)
	{
		WriteLine(String::Format(Format, std::forward<TArgs>(InArgs)...));
	}
};