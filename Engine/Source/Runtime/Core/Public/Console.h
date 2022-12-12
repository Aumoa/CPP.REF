// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "ConsoleColor.h"
#include "Misc/String.h"
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