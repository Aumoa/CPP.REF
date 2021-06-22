// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Rook.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"

ARook::ARook() : Super()
{
}

StaticMesh* ARook::GetStaticMesh() const
{
	static StaticMesh* globalMesh = GameEngine::GetEngine()->GetAssetImporter()->ImportStaticMesh(AssetPath);
	return globalMesh;
}