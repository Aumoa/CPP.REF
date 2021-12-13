// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Console.h"
#include <iostream>
#include <string>

#define SCOPED_LOCK() std::unique_lock Lock(Locker)

std::mutex Console::Locker;

void Console::Write(std::wstring_view Message)
{
	SCOPED_LOCK();
	std::wcout << Message;
}

void Console::WriteLine(std::wstring_view Message)
{
	SCOPED_LOCK();
	std::wcout << Message << std::endl;
}

std::wstring Console::ReadLine()
{
	SCOPED_LOCK();
	std::wstring Line;
	std::getline(std::wcin, Line);
	return Line;
}

void Console::Clear()
{
	SCOPED_LOCK();
	std::system("cls");
}

#undef SCOPED_LOCK