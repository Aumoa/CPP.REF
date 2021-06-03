// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <crtdbg.h>
#include <Windows.h>

import std.core;
import SC.Game.Chess;
import SC.Platform.Windows;

using namespace std::chrono;

INT __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR cmd, INT show)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & ~_CRTDBG_ALLOC_MEM_DF & ~_CRTDBG_LEAK_CHECK_DF);
	zoned_time leak_guard = system_clock::now();
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	ChessGameInstance gameInstance;
	return gameInstance.Run(gameInstance.CreateSubobject<CoreWindow>());
}