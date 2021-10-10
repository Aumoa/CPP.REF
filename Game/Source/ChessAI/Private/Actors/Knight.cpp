// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Actors/Knight.h"
#include "Assets/StaticMesh.h"
#include "Actors/ChessBoard.h"

AKnight::AKnight() : Super()
{
}

bool AKnight::QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const
{
	GridIndex direction[] =
	{
		// Left top
		GridIndex(-2, +1), GridIndex(-1, +2),

		// Right top
		GridIndex(+1, +2), GridIndex(+2, +1),

		// Right bottom
		GridIndex(+2, -1), GridIndex(+1, -2),

		// Left bottom
		GridIndex(-1, -2), GridIndex(-2, -1),
	};

	for (int32 i = 0; i < _countof(direction); ++i)
	{
		GridIndex loc = GetIndex() + direction[i];
		query.BeginFigure(MovablePointsArray::FigureType::Move)->CheckAndEmplace(this, loc, built);
		query.BeginFigure(MovablePointsArray::FigureType::Attack)->CheckAndEmplace(this, loc, built);
	}

	query.OwnerActor = this;
	return true;
}

SStaticMesh* AKnight::GetStaticMesh() const
{
	static SStaticMesh* globalMesh = LoadObject<SStaticMesh>(AssetPath);
	return globalMesh;
}