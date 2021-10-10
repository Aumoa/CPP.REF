// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Actors/Rook.h"
#include "Assets/StaticMesh.h"
#include "Queries/DirectionalMovablePiece.h"

ARook::ARook() : Super()
{
}

bool ARook::QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const
{
	return DirectionalMovablePiece::QueryMovable(this, query, built, true, false, 8);
}

SStaticMesh* ARook::GetStaticMesh() const
{
	static SStaticMesh* globalMesh = LoadObject<SStaticMesh>(AssetPath);
	return globalMesh;
}