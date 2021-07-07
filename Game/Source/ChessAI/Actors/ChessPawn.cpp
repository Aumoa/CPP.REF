// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessPawn.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"

using namespace std;

AChessPawn::AChessPawn() : Super()
{
}

bool AChessPawn::QueryMovable(MovablePointsQuery& query) const
{
	int32 incrementer = GetIncrementer();
	MovablePointsArrayPointer figure = query.BeginFigure(MovablePointsArray::FigureType::Move);

	GridIndex location = GetIndex();
	location.Y += GetIncrementer();
	const bool bBlocked = !figure->CheckAndEmplace(this, location);

	if (IsFirst() && !bBlocked)
	{
		figure->CheckAndEmplace(this, location + GridIndex(0, GetIncrementer()));
	}

	GridIndex left = location + GridIndex(-1, 0);
	GridIndex right = location + GridIndex(+1, 0);
	query.BeginFigure(MovablePointsArray::FigureType::Attack)->CheckAndEmplace(this, left);
	query.BeginFigure(MovablePointsArray::FigureType::Attack)->CheckAndEmplace(this, right);

	query.OwnerActor = this;
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