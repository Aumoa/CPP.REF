// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "WindowsLaunch.h"
#include "Console.h"
#include <iostream>

int32 NWindowsLaunch::Main(std::vector<String> Args)
{
	Console::WriteLine(String::Format(TEXT("Arguments: {}"), Args[0]));
	int32 A;
	std::cin >> A;
	return 0;
}