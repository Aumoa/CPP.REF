// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Assets/StaticMesh/StaticMesh.h"
#include "LogGame.h"

GENERATE_BODY(SStaticMesh);

SStaticMesh::SStaticMesh() : Super()
{
}

bool SStaticMesh::StreamIn(SAssetsLoader* Loader, std::span<const uint8> AssetsBin)
{
	checkf(false, L"NOT IMPLEMENTED");
	return false;
}