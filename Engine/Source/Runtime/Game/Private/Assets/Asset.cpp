// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/Asset.h"
#include "Misc/Paths.h"

SAsset::SAsset(const std::filesystem::path& path) : Super()
	, _assetPath(path)
{
}

SAsset::~SAsset()
{
}

std::filesystem::path SAsset::GetAssetPath() const
{
	return _assetPath;
}

size_t SAsset::AddRef()
{
	return ++_lockCount;
}

size_t SAsset::ReleaseRef()
{
	return --_lockCount;
}