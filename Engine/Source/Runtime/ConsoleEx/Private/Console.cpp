// Copyright 2017 CLOVERGAMES Co., Ltd. All Rights Reserved.

#include "Console.h"
#include <iostream>
#include "Console.gen.cpp"

void Console::Write(const String& format)
{
	std::wcout << format.c_str();
}