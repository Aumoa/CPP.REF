// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Game;
import SC.Game.Chess;
import SC.Game.ChessAI;

GameLevel::GameLevel() : Super()
{
}

void GameLevel::LoadLevel(World* world)
{
	_psBoard = world->SpawnActor<AChessBoard>(L"ChessBoard");
}