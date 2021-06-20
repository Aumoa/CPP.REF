// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "AssetImporter.h"
#include "Parser/AssimpParser.h"
#include "Assets/StaticMesh.h"

using namespace std::filesystem;

AssetImporter::AssetImporter(GameEngine* engine, RHIVertexFactory* factory) : Super()
	, _engine(engine)
	, _factory(factory)
{
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