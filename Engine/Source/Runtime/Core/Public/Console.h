// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "ConsoleColor.h"
#include "CoreTypes/String.h"
#include "Misc/StaticClass.h"
#include "IO/TextWriter.h"

class CORE_API Console : public StaticClass
{
public:
	static void Write(String Str);
	static void WriteLine(String Str);

	static String ReadLine();

	static void SetForegroundColor(EConsoleColor InColor);
	static EConsoleColor GetForegroundColor();

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

public:
	static TextWriter& Out;
	static TextWriter& Error;
};