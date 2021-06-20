// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "GameFramework/Pawn.h"

class AChessBoard;

class AChessBoardProxy : public APawn
{
public:
	using Super = APawn;

private:
	AChessBoard* _board = nullptr;

public:
	AChessBoardProxy(AChessBoard* chessBoard);

	void SetBoard(AChessBoard* board) { _board = board; }
};