// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ChessAIStructures.h"
#include "Queries/ChessQueries.h"

class AChessBoard;

class CHESSAI_API AChessBoardProxy : public APawn
{
	GENERATED_BODY(AChessBoardProxy)

private:
	AChessBoard* _board = nullptr;
	EChessTeam _myteam = EChessTeam::Black;

public:
	AChessBoardProxy(AChessBoard* chessBoard);

	void InitBoard(AChessBoard* board, EChessTeam team);
	inline AChessBoard* GetBoard() const { return _board; }

	bool CanSelect(const GridIndex& location) const;
	ActionRecord SimulateMove(const GridIndex& from, const GridIndex& to);
};