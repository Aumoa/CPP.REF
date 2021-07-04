// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Queen.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"

AQueen::AQueen() : Super()
{
}

bool AQueen::QueryMovable(MovablePointsQuery& query) const
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

StaticMesh* AQueen::GetStaticMesh() const
{
	static StaticMesh* globalMesh = GameEngine::GetEngine()->GetAssetImporter()->ImportStaticMesh(AssetPath);
	return globalMesh;
}