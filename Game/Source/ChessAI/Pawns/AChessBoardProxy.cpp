// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Game;
import SC.Game.ChessAI;

AChessBoardProxy::AChessBoardProxy(AChessBoard* chessBoard) : Super()
{
	CameraComponent* cp = CreateSubobject<CameraComponent>();
	SetRootComponent(cp);
}