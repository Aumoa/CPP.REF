// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessPawn.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"

AChessPawn::AChessPawn() : Super()
{
}

StaticMesh* AChessPawn::GetStaticMesh() const
{
	static StaticMesh* globalMesh = GameEngine::GetEngine()->GetAssetImporter()->ImportStaticMesh(AssetPath);
	return globalMesh;
}