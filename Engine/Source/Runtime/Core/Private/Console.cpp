// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Console.h"
#include <cstdio>

void Console::Write(String message)
{
	wprintf(message.c_str());
}

void Console::WriteLine(String message)
{
	static thread_local std::vector<char_t> sBuf;
	sBuf.resize(message.length() + 2);
	memcpy(sBuf.data(), message.c_str(), sizeof(char_t) * message.length());
	sBuf[message.length()] = '\n';
	sBuf[message.length() + 1] = 0;
	wprintf(sBuf.data());
}