// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Game.ChessAI:APiece;

import std.core;
import SC.Runtime.Game;
import :AChessBoard;

using namespace std;

export class APiece : public AActor
{
public:
	using Super = AActor;

private:
	AChessBoard* _board = nullptr;

public:
	APiece(AChessBoard* board) : Super()
		, _board(board)
	{
	}
};