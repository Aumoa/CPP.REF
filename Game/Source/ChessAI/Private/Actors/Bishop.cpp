// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Actors/Bishop.h"
#include "GameEngine.h"
#include "Assets/StaticMesh.h"
#include "Queries/DirectionalMovablePiece.h"

ABishop::ABishop() : Super()
{
}

bool ABishop::QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const
{
	return DirectionalMovablePiece::QueryMovable(this, query, built, false, true, 8);
}

SStaticMesh* ABishop::GetStaticMesh() const
{
	static SStaticMesh* globalMesh = LoadObject<SStaticMesh>(AssetPath);
	return globalMesh;
}