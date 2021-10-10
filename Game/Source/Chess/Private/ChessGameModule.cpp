// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessGameModule.h"
#include "ChessGameInstance.h"

SGameInstance* SChessGameModule::CreatePrimaryGameModule()
{
	return NewObject<ChessGameInstance>();
}

DEFINE_GAME_MODULE(SChessGameModule);