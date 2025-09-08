// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS
#define __ALLOW_PLATFORM_COMMON_H__

#include "CoreMinimal.h"
#include "Launch.h"
#include "CommandLineParser.h"
#include "Platform/PlatformCommon.h"
#include "Platform/DynamicLibrary.h"

INT APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	using namespace ::Ayla;
	DynamicLibrary apiSet(TEXT("WindowsAPI"));
	auto args = String::FromLiteral(lpCmdLine);

	return Launch::GuardedMain(std::make_unique<CommandLineParser>(args), apiSet);
}

#undef __ALLOW_PLATFORM_COMMON_H__
#endif