// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Game.ChessAI;

AChessBoard::AChessBoard() : Super(L"ChessBoard")
{
}

AChessBoardProxy* AChessBoard::CreateProxy()
{
	return CreateSubobject<AChessBoardProxy>(this);
}