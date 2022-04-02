// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/String.h"
#include <span>
#include <iostream>
#include <vector>

using namespace libty;

int32 GuardedMain(std::span<std::wstring> argv)
{
	return TApplicationClass::GuardedMain(argv);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");

	std::vector<std::wstring> Argv(argc);
	for (size_t i = 0; i < Argv.size(); ++i)
	{
		Argv[i] = String::AsUnicode(argv[i]);
	}

	return GuardedMain(Argv);
}