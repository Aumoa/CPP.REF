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
				_assets.emplace(entry.path(), nullptr);
			}
		}
	}
}

StaticMesh* AssetImporter::ImportStaticMesh(const std::filesystem::path& importPath)
{
	AssimpParser aParse(_engine, _factory);
	if (!aParse.TryParse(importPath))
	{
		return nullptr;
	}
	
	StaticMesh* mesh = aParse.GetStaticMesh();
	mesh->SetOuter(this);
	return mesh;
}