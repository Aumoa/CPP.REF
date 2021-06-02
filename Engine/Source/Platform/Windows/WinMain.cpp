// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>
#include <crtdbg.h>

import std.core;
import SC.Runtime.Core;
import SC.Game.Chess;
import SC.Platform.Windows;

using namespace std::chrono;

INT APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR cmd, INT show)
{	
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & ~_CRTDBG_ALLOC_MEM_DF & ~_CRTDBG_LEAK_CHECK_DF);
	zoned_time leak_guard = system_clock::now();
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	ChessGameInstance gameInstance;
	return gameInstance.Run(gameInstance.CreateSubobject<CoreWindow>());
}