// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_LINUX
#define __ALLOW_PLATFORM_COMMON_H__

#include "CoreMinimal.h"
#include "Launch.h"
#include "CommandLineParser.h"
#include "Platform/PlatformCommon.h"
#include "Platform/DynamicLibrary.h"

int main(int argc, char* argv[])
{
	using namespace ::Ayla;
	DynamicLibrary apiSet(TEXT("LinuxAPI"));
	std::vector<String> args{ (size_t)argc };
	for (int i = 0; i < argc; ++i)
	{
		args[i] = String::FromLiteral(std::string_view(argv[i]));
	}

	int32 exitCode = Launch::GuardedMain(std::make_unique<CommandLineParser>(std::move(args)), apiSet);
	return exitCode;
}

#undef __ALLOW_PLATFORM_COMMON_H__
#endif
