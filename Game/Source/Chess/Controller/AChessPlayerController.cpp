// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Game;
import SC.Game.Chess;
import SC.Game.ChessAI;

AChessPlayerController::AChessPlayerController() : Super()
{
}

void AChessPlayerController::BeginPlay()
{
	Super::BeginPlay();

	World* world = GetWorld();
	Level* level = world->GetLevel();
	GameLevel* gameLevel = dynamic_cast<GameLevel*>(level);
	if (gameLevel != nullptr)
	{
		AChessBoard* board = gameLevel->GetPersistentChessBoard();
		AChessBoardProxy* proxy = board->CreateProxy();
		Possess(proxy);

		proxy->SetActorLocation(Vector3(0, 0, -10));
	}
}