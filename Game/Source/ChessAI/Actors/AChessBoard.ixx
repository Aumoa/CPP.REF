// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Game.ChessAI:AChessBoard;

import SC.Runtime.Game;

export class AChessBoardProxy;

export class AChessBoard : public AActor
{
public:
	using Super = AActor;

public:
	AChessBoard();

	AChessBoardProxy* CreateProxy();
};