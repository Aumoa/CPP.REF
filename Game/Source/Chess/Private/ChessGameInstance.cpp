// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessGameInstance.h"
#include "Level/GameLevel.h"

ChessGameInstance::ChessGameInstance() : Super()
{
	StartupLevel = GameLevel::StaticClass();
}