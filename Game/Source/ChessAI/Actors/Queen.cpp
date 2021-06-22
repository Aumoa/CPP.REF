// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Queen.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"

AQueen::AQueen() : Super()
{
}

StaticMesh* AQueen::GetStaticMesh() const
{
	static StaticMesh* globalMesh = GameEngine::GetEngine()->GetAssetImporter()->ImportStaticMesh(AssetPath);
	return globalMesh;
}