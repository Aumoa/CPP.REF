// Copyright 2020 Aumoa.lib. All right reserved.

#include "Assets/AssetManager.h"

#include "Logging/LogVerbosity.h"
#include "Logging/LogCategoryBase.h"
#include "SceneRendering/StaticMesh.h"

using namespace std;

LogCategoryBase AssetManager::LogAssets(ELogVerbosity::Verbose, L"LogAssets");

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}

void AssetManager::Import(TRefPtr<String> key, TRefPtr<StaticMesh> imported)
{
	assets.insert(make_pair(key, imported));
}

StaticMesh* AssetManager::Load(TRefPtr<String> key) const
{
	auto it = assets.find(key);
	if (it == assets.end())
	{
		return nullptr;
	}

	return it->second.Get();
}