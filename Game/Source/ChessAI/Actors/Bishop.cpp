// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Bishop.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"

ABishop::ABishop() : Super()
{
}

StaticMesh* ABishop::GetStaticMesh() const
{
	static StaticMesh* globalMesh = GameEngine::GetEngine()->GetAssetImporter()->ImportStaticMesh(AssetPath);
	return globalMesh;
}