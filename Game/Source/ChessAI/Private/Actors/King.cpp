// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Actors/King.h"
#include "Assets/StaticMesh.h"
#include "Queries/DirectionalMovablePiece.h"

AKing::AKing() : Super()
{
}

bool AKing::QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const
{
	return DirectionalMovablePiece::QueryMovable(this, query, built, true, true, 2);
}

SStaticMesh* AKing::GetStaticMesh() const
{
	static SStaticMesh* globalMesh = LoadObject<SStaticMesh>(AssetPath);
	return globalMesh;
}