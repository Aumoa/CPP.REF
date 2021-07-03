// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessSystemComponent.h"
#include "Actors/ChessBoard.h"
#include "Pawns/ChessBoardProxy.h"

ChessSystemComponent::ChessSystemComponent() : Super()
{
}

void ChessSystemComponent::SetupBoard(AChessBoardProxy* board)
{
	_board = board->GetBoard();
	_boardProxy = board;
}