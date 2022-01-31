// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "StaticMesh/StaticMesh.h"
#include "LogAssetsIO.h"

GENERATE_BODY(SStaticMesh);

SStaticMesh::SStaticMesh() : Super()
{
}

bool SStaticMesh::StreamIn(SAssetsLoader* Loader, std::span<const uint8> AssetsBin)
{
	checkf(false, L"NOT IMPLEMENTED");
	return false;
}