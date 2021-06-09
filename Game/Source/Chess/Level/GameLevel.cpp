// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Game;
import SC.Game.Chess;
import SC.Game.ChessAI;

GameLevel::GameLevel() : Super()
{
}

bool GameLevel::LoadLevel(World* world)
{
	if (!Super::LoadLevel(world))
	{
		return false;
	}

	_psBoard = world->SpawnActor<AChessBoard>();
	return true;
}