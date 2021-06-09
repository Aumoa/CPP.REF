// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Game.ChessAI;

using namespace std;

AChessBoard::AChessBoard() : Super()
{
}

AChessBoardProxy* AChessBoard::CreateProxy()
{
	return CreateSubobject<AChessBoardProxy>(this);
}