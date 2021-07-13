// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <crtdbg.h>
#include <Windows.h>
#include "THGameInstance.h"
#include "CoreWindow.h"

using namespace std::chrono;

INT __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR cmd, INT show)
{	
	THGameInstance gameInstance;
	return gameInstance.Run(gameInstance.CreateSubobject<CoreWindow>());
}