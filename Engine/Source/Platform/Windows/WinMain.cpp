// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>
#include <crtdbg.h>

import SC.Runtime.Core;
import SC.Game.Chess;
import SC.Platform.Windows;

INT APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR cmd, INT show)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	ChessGameInstance gameInstance;
	return gameInstance.Run(gameInstance.CreateSubobject<CoreWindow>());
}