// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectionalMovablePiece.h"
#include "Actors/Piece.h"

DirectionalMovablePiece::DirectionalMovablePiece()
{
}

bool DirectionalMovablePiece::QueryMovable(const APiece* piece, MovablePointsQuery& query, bool bOrthogonal, bool bDiagonal, int32 length)
{
	GridIndex direction[] =
	{
		// Orthogonal
		GridIndex(-1, +0), GridIndex(+1, +0),
		GridIndex(+0, -1), GridIndex(+0, +1),

		// Diagonal
		GridIndex(-1, +1), GridIndex(+1, +1),
		GridIndex(-1, -1), GridIndex(+1, -1),
	};

	const int32 Length = 8;
	for (int32 i = 0; i < _countof(direction); ++i)
	{
		MovablePointsArrayPointer figure = query.BeginFigure(MovablePointsArray::FigureType::Move);
		MovablePointsArrayPointer figureAttack = query.BeginFigure(MovablePointsArray::FigureType::Attack);
		for (int32 j = 1; j < Length; ++j)
		{
			GridIndex loc = piece->GetIndex() + direction[i] * j;
			if (!figure->CheckAndEmplace(piece, loc))
			{
				figureAttack->CheckAndEmplace(piece, loc);
				break;
			}
			else
			{
				figureAttack->CheckAndEmplace(piece, loc);
			}
		}
	}

	query.OwnerActor = piece;
	return true;
}