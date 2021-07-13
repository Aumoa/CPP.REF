// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <crtdbg.h>
#include <Windows.h>
#include "THGameInstance.h"
#include "CoreWindow.h"

using namespace std::chrono;

INT __stdcall wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR cmd, _In_ INT show)
{	
	THGameInstance gameInstance;
	return gameInstance.Run(gameInstance.CreateSubobject<CoreWindow>());
}