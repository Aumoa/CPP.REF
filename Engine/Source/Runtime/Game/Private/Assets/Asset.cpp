// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/Asset.h"
#include "Misc/Paths.h"

Asset::Asset(const std::filesystem::path& path) : Super()
	, _assetPath(path)
{
}

Asset::~Asset()
{
}

std::filesystem::path Asset::GetAssetPath() const
{
	return _assetPath;
}