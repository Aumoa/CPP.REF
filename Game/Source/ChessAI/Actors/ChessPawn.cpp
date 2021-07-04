// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessPawn.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"

AChessPawn::AChessPawn() : Super()
{
}

bool AChessPawn::SimulateMove(const GridIndex& index)
{
	bool bMoved = Super::SimulateMove(index);
	if (bMoved)
	{
		_bFirst = false;
	}
	return bMoved;
}

bool AChessPawn::QueryMovable(MovablePointsQuery& query) const
{
	int32 incrementer = GetIncrementer();
	MovablePointsArray* figure = query.BeginFigure(MovablePointsArray::FigureType::Move);

	GridIndex location = GetIndex();
	location.Y += GetIncrementer();
	const bool bBlocked = !CheckAndEmplace(figure, location);

	GridIndex left = location + GridIndex(-1, 0);
	GridIndex right = location + GridIndex(+1, 0);
	CheckAndEmplaceHit(query, left);
	CheckAndEmplaceHit(query, right);

	if (_bFirst && !bBlocked)
	{
		location.Y += GetIncrementer();
		CheckAndEmplace(figure, location);
	}

	return true;
}

StaticMesh* AChessPawn::GetStaticMesh() const
{
	static StaticMesh* globalMesh = GameEngine::GetEngine()->GetAssetImporter()->ImportStaticMesh(AssetPath);
	return globalMesh;
}

inline int32 AChessPawn::GetIncrementer() const
{
	switch (GetTeam())
	{
	case EChessTeam::Black:
		return -1;
	case EChessTeam::White:
		return +1;
	}

	checkf(false, L"Invalid team.");
	return 0;
}

void AChessPawn::QueryMove(MovablePointsQuery& query) const
{
}

void AChessPawn::QueryAttack(MovablePointsQuery& query) const
{
}