// Copyright 2020-2025 Aumoa.lib. All right reserved.

#define __ALLOW_PLATFORM_COMMON_H__

#include "CoreMinimal.h"
#include "Platform/PlatformCommon.h"
#include "Launch.h"
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

	return Launch::GuardedMain(std::move(args), apiSet, nullptr);
}

#undef __ALLOW_PLATFORM_COMMON_H__
