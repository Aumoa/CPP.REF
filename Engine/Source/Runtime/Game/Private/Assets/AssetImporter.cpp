// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/AssetImporter.h"
#include "GameEngine.h"
#include "Assets/StaticMesh.h"
#include "Assets/Parser/AssimpParser.h"

AssetImporter::AssetImporter(GameEngine* engine, RHIVertexFactory* factory) : Super()
	, _engine(engine)
	, _factory(factory)
{
}

void AssetImporter::SearchContents()
{
	std::queue<std::filesystem::directory_iterator> iterators;
}

std::filesystem::path AssetImporter::GetContentDir() const
{
	return std::filesystem::path(L"Content");
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