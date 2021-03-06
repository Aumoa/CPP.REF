// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "King.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"
#include "Queries/DirectionalMovablePiece.h"

AKing::AKing() : Super()
{
}

bool AKing::QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const
{
	return DirectionalMovablePiece::QueryMovable(this, query, built, true, true, 2);
}

StaticMesh* AKing::GetStaticMesh() const
{
	static StaticMesh* globalMesh = GameEngine::GetEngine()->GetAssetImporter()->ImportStaticMesh(AssetPath);
	return globalMesh;
}