// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "Queries/ChessQueries.h"

class APiece;

class DirectionalMovablePiece
{
public:
	DirectionalMovablePiece();

	static bool QueryMovable(const APiece* piece, MovablePointsQuery& query, const ChessBoardBuilt& built, bool bOrthogonal, bool bDiagonal, int32 length);
};