// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>

import SC.Runtime.Core;
import SC.Game.Chess;
import SC.Platform.Windows;

INT APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR cmd, INT show)
{
	ChessGameInstance gameInstance;
	return gameInstance.Run(gameInstance.CreateSubobject<CoreWindow>());
}