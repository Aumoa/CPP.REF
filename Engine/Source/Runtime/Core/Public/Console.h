// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "ConsoleColor.h"
#include "CoreTypes/String.h"
#include "Misc/StaticClass.h"

class CORE_API Console : public StaticClass
{
public:
	static void Write(String message);
	static void WriteLine(String message);
	static void SetForegroundColor(EConsoleColor color);
	static EConsoleColor GetForegroundColor();

private:
	static void _trap_init();
};