// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "Console.h"
#include "ComPtr.h"
#include "Helper.h"
#include <iostream>

using namespace Ayla;

int main(int argc, char** argv)
{
	std::vector<String> args;
	args.reserve((size_t)argc);
	for (int i = 0; i < argc; ++i)
	{
		args.emplace_back(String::FromLiteral(argv[i]));
	}

	ComPtr<IDxcCompiler> compiler;
	HR(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler)));

	Console::WriteLine(TEXT("{}"), String::Join(TEXT(", "), args));
	auto s = ReadLine();
	Console::WriteLine(s);
	return 0;
}