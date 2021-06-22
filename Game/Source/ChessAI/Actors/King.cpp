// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "King.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"

AKing::AKing() : Super()
{
}

StaticMesh* AKing::GetStaticMesh() const
{
	static StaticMesh* globalMesh = GameEngine::GetEngine()->GetAssetImporter()->ImportStaticMesh(AssetPath);
	return globalMesh;
}