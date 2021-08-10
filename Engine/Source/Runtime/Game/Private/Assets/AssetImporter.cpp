// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/AssetImporter.h"
#include "GameEngine.h"
#include "Assets/StaticMesh.h"
#include "Assets/Parser/AssimpParser.h"

using namespace std;
using namespace std::filesystem;

AssetImporter::AssetImporter(GameEngine* engine, RHIVertexFactory* factory) : Super()
	, _engine(engine)
	, _factory(factory)
{
}

void AssetImporter::SearchContents()
{
	queue<directory_iterator> iterators;
}

path AssetImporter::GetContentDir() const
{
	return path(L"Content");
}

StaticMesh* AssetImporter::ImportStaticMesh(const path& importPath)
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