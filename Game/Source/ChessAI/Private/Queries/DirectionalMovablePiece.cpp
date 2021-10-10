// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Queries/DirectionalMovablePiece.h"
#include "Actors/Piece.h"

DirectionalMovablePiece::DirectionalMovablePiece()
{
}

bool DirectionalMovablePiece::QueryMovable(const APiece* piece, MovablePointsQuery& query, const ChessBoardBuilt& built, bool bOrthogonal, bool bDiagonal, int32 length)
{
	std::vector<GridIndex> direction;
	direction.reserve(8);

	if (bOrthogonal)
	{
		direction.emplace_back(-1, +0);
		direction.emplace_back(+1, +0);
		direction.emplace_back(+0, -1);
		direction.emplace_back(+0, +1);
	}

	if (bDiagonal)
	{
		direction.emplace_back(-1, +1);
		direction.emplace_back(+1, +1);
		direction.emplace_back(-1, -1);
		direction.emplace_back(+1, -1);
	}

	const int32 Length = length;
	for (int32 i = 0; i < (int32)direction.size(); ++i)
	{
		MovablePointsArrayPointer figure = query.BeginFigure(MovablePointsArray::FigureType::Move);
		MovablePointsArrayPointer figureAttack = query.BeginFigure(MovablePointsArray::FigureType::Attack);
		for (int32 j = 1; j < Length; ++j)
		{
			GridIndex loc = piece->GetIndex() + direction[i] * j;
			if (!figure->CheckAndEmplace(piece, loc, built))
			{
				figureAttack->CheckAndEmplace(piece, loc, built);
				break;
			}
			else
			{
				figureAttack->CheckAndEmplace(piece, loc, built);
			}
		}
	}

	query.OwnerActor = piece;
	return true;
}