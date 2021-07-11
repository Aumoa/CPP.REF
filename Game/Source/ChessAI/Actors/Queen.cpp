// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Queen.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"
#include "Queries/DirectionalMovablePiece.h"

AQueen::AQueen() : Super()
{
}

bool AQueen::QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const
{
	return DirectionalMovablePiece::QueryMovable(this, query, built, true, true, 8);
}

StaticMesh* AQueen::GetStaticMesh() const
{
	static StaticMesh* globalMesh = GameEngine::GetEngine()->GetAssetImporter()->ImportStaticMesh(AssetPath);
	return globalMesh;
}