// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"

class AChessBoard;

class APiece : public AActor
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