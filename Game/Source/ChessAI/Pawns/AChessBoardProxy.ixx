// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Game.ChessAI:AChessBoardProxy;

import SC.Runtime.Game;

export class AChessBoard;

export class AChessBoardProxy : public APawn
{
public:
	using Super = APawn;

private:
	AChessBoard* _board = nullptr;

public:
	AChessBoardProxy(AChessBoard* chessBoard);
};