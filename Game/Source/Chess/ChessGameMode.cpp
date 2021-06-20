// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessGameMode.h"
#include "Controller/ChessPlayerController.h"

AChessGameMode::AChessGameMode() : Super()
{
	PlayerControllerClass = SubclassOf<AChessPlayerController>::StaticClass();
}