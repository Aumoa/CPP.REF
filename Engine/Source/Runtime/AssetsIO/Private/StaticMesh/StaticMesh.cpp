// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "StaticMesh/StaticMesh.h"
#include "LogAssetsIO.h"

SStaticMesh::SStaticMesh() : Super()
{
}

bool SStaticMesh::StreamIn(SAssetsLoader* Loader, std::span<const uint8> AssetsBin)
{
	check(false);
	return false;
}