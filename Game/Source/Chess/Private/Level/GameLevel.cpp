// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Level/GameLevel.h"
#include "ChessGameMode.h"
#include "Level/World.h"
#include "Actors/ChessBoard.h"

GameLevel::GameLevel() : Super()
{
	GameModeClass = AChessGameMode::StaticClass();
}

void GameLevel::OnLoadLevel()
{
	_psBoard = Cast<AChessBoard>(SpawnActor(AChessBoard::StaticClass(), true));
	_psBoard->SetActorLocation(Vector3(3.5f, 0, 3.5f));
	_psBoard->InitBoard(GetWorld());
}