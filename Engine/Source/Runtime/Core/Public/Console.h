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
	static void SetForegroundColor(EConsoleColor color);
	static EConsoleColor GetForegroundColor();

public:
	static TextWriter& Out;
	static TextWriter& Error;

private:
	static void _trap_init();
};