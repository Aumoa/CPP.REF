// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Assets/AssetManager.h"

#include "Logging/LogVerbosity.h"
#include "Logging/LogCategoryBase.h"
#include "SceneRendering/StaticMesh.h"
#include "Materials/Material.h"

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
	staticMeshes.insert(make_pair(key, imported));
}

StaticMesh* AssetManager::LoadStaticMesh(TRefPtr<String> key) const
{
	auto it = staticMeshes.find(key);
	if (it == staticMeshes.end())
	{
		return nullptr;
	}

	return it->second.Get();
}

void AssetManager::Import(TRefPtr<String> key, TRefPtr<Material> imported)
{
	materials.insert(make_pair(key, imported));
}

Material* AssetManager::LoadMaterial(TRefPtr<String> key) const
{
	auto it = materials.find(key);
	if (it == materials.end())
	{
		return nullptr;
	}

	return it->second.Get();
}