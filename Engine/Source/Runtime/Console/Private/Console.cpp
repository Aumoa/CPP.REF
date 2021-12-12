// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Console.h"
#include <iostream>

void Console::Write(std::wstring_view Message)
{
	std::wcout << Message;
}

void Console::WriteLine(std::wstring_view Message)
{
	std::wcout << Message << std::endl;
}