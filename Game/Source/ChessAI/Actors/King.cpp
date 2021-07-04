// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "King.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"

AKing::AKing() : Super()
{
}

bool AKing::QueryMovable(MovablePointsQuery& query) const
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

	const int32 Length = 2;
	for (int32 i = 0; i < _countof(direction); ++i)
	{
		MovablePointsArray* figure = query.BeginFigure(MovablePointsArray::FigureType::Move);
		for (int32 j = 1; j < Length; ++j)
		{
			GridIndex loc = GetIndex() + direction[i] * j;
			if (!CheckAndEmplace(figure, loc))
			{
				CheckAndEmplaceHit(query, loc);
				break;
			}
		}
	}

	return true;
}

StaticMesh* AKing::GetStaticMesh() const
{
	static StaticMesh* globalMesh = GameEngine::GetEngine()->GetAssetImporter()->ImportStaticMesh(AssetPath);
	return globalMesh;
}