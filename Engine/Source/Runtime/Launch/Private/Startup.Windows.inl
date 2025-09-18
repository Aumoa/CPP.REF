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
	auto args = String::FromLiteral(lpCmdLine);
	return Launch::Main(std::make_unique<CommandLineParser>(args), TEXT("WindowsAPI"));
}

#undef __ALLOW_PLATFORM_COMMON_H__
#endif