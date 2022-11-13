// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Console.h"
#include <cstdio>

void Console::Write(String message)
{
	static int _trap = (_trap_init(), 0);

	wprintf(message.c_str());
}

void Console::WriteLine(String message)
{
	static int _trap = (_trap_init(), 0);

	static thread_local std::vector<char_t> sBuf;
	sBuf.resize(message.length() + 2);
	memcpy(sBuf.data(), message.c_str(), sizeof(char_t) * message.length());
	sBuf[message.length()] = '\n';
	sBuf[message.length() + 1] = 0;
	wprintf(sBuf.data());
}

void Console::_trap_init()
{
	static int _trap = []()
	{
		_wsetlocale(LC_ALL, L"korean");
		return 0;
	}();
}