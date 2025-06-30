// Copyright 2020-2025 Aumoa.lib. All right reserved.

#define __ALLOW_PLATFORM_COMMON_H__

#include "CoreMinimal.h"
#include "Platform/PlatformCommon.h"
#include "Launch.h"
#include "Platform/DynamicLibrary.h"

INT APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	using namespace ::Ayla;
	DynamicLibrary apiSet(TEXT("WindowsAPI"));
	auto args = String::FromLiteral(lpCmdLine)
		.Split(TEXT(" "), StringSplitOptions::RemoveEmptyEntries | StringSplitOptions::TrimEntries);
	
	return Launch::GuardedMain(std::move(args), apiSet);
}

#undef __ALLOW_PLATFORM_COMMON_H__
