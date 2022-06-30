// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "CoreMinimal.h"
#include "GameApp.h"
#include <Windows.h>

INT APIENTRY WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ char* args, _In_ INT)
{
	return GameAppCore::GuardedMain(typeof(GameApp), CommandLineBuilder(args));
}

#endif