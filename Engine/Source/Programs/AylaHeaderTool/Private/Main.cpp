// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "WindowsLaunch.h"

int main(int Argc, char** Argv)
{
	std::vector<String> Args;
	Args.reserve(Argc);

	for (int i = 0; i < Argc; ++i)
	{
		Args.emplace_back(String::FromCodepage(Argv[i]));
	}
	
	return NWindowsLaunch::Main(std::move(Args));
}