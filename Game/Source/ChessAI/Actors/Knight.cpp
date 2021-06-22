// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Knight.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"

AKnight::AKnight() : Super()
{
}

StaticMesh* AKnight::GetStaticMesh() const
{
	static StaticMesh* globalMesh = GameEngine::GetEngine()->GetAssetImporter()->ImportStaticMesh(AssetPath);
	return globalMesh;
}