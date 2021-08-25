// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/AssetImporter.h"
#include "Assets/StaticMesh.h"
#include "Assets/Parser/AssimpParser.h"
#include "GameEngine.h"
#include "LogGame.h"
#include "Misc/Paths.h"

AssetImporter::AssetImporter(GameEngine* engine, RHIVertexFactory* factory) : Super()
	, _engine(engine)
	, _factory(factory)
{
}

void AssetImporter::SearchContents()
{
	using namespace std::filesystem;

	std::queue<directory_iterator> iterators;
	iterators.emplace(directory_iterator(Paths::GetContentPath()));

	for (directory_iterator it; !iterators.empty(); iterators.pop())
	{
		it = iterators.front();

		for (auto& entry : it)
		{
			if (entry.is_directory())
			{
				iterators.emplace(entry.path());
			}
			else
			{
				_assets.emplace(entry.path(), LoadedObjectData());
			}
		}
	}
}

Asset* AssetImporter::LoadObject(const std::filesystem::path& importPath)
{
	auto find_it = _assets.find(importPath);
	if (find_it == _assets.end())
	{
		SE_LOG(LogAssets, Error, L"Could not found asset path: {}", importPath.wstring());
		return nullptr;
	}

	LoadedObjectData& data = find_it->second;
	if (data.Ptr == nullptr)
	{
		if (!importPath.has_extension())
		{
			SE_LOG(LogAssets, Error, L"It is not valid file extension for parsing.");
			return nullptr;
		}

		std::filesystem::path ext = importPath.extension();
		if (ext == L"jpg")
		{
			SE_LOG(LogAssets, Error, L"Could not supports jpg format yet.");
			return nullptr;
		}
		else
		{
			data.Ptr = LoadAssimpObject(importPath);
		}
	}

	return data.Ptr;
}

void AssetImporter::UnloadObject(const std::filesystem::path& importPath)
{
	auto find_it = _assets.find(importPath);
	if (find_it == _assets.end())
	{
		SE_LOG(LogAssets, Error, L"Could not found asset path: {}", importPath.wstring());
		return;
	}

	LoadedObjectData& data = find_it->second;
	if (data.Ptr)
	{
		DestroySubobject(data.Ptr);
		data.Ptr = nullptr;
	}
}

Asset* AssetImporter::LoadAssimpObject(const std::filesystem::path& importPath)
{
	AssimpParser assimp(_engine, _factory);
	if (!assimp.TryParse(importPath))
	{
		SE_LOG(LogAssets, Error, L"Could not parsing asset from file: {}", importPath.wstring());
		return nullptr;
	}

	StaticMesh* loadedObject = assimp.GetStaticMesh();
	loadedObject->SetOuter(this);

	return loadedObject;
}