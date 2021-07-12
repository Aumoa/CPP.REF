// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessBoardProxy.h"
#include "Camera/CameraComponent.h"
#include "Actors/ChessBoard.h"
#include "Actors/Piece.h"

AChessBoardProxy::AChessBoardProxy(AChessBoard* chessBoard) : Super()
	, _board(chessBoard)
{
	CameraComponent* cp = CreateSubobject<CameraComponent>();
	SetRootComponent(cp);
}

void AChessBoardProxy::InitBoard(AChessBoard* board, EChessTeam team)
{
	_board = board;
	_myteam = team;
}

bool AChessBoardProxy::CanSelect(const GridIndex& location) const
{
	APiece* piece = _board->GetBoardBuilt().GetPiece(location);
	if (piece == nullptr)
	{
		return false;
	}

	EChessTeam team = piece->GetTeam();
	if (team != _myteam)
	{
		//return false;
	}

	return true;
}

ActionRecord AChessBoardProxy::SimulateMove(const GridIndex& from, const GridIndex& to)
{
	//if (_board->GetTurn() != _myteam)
	//{
	//	return false;
	//}

	return _board->MovePiece(from, to);
}