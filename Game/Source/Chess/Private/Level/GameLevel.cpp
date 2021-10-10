// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Level/GameLevel.h"
#include "ChessGameMode.h"
#include "Level/World.h"
#include "Actors/ChessBoard.h"

GameLevel::GameLevel() : Super()
{
	GameModeClass = AChessGameMode::StaticClass();
}

bool GameLevel::LoadLevel(SWorld* world)
{
	if (!Super::LoadLevel(world))
	{
		return false;
	}

	_psBoard = world->SpawnActor<AChessBoard>();
	_psBoard->SetActorLocation(Vector3(3.5f, 0, 3.5f));
	_psBoard->InitBoard(world);

	return true;
}