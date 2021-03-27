// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Assets/AssetImporter.h"

#include "Parser/AssimpParser.h"
#include "SceneRendering/StaticMesh.h"

AssetImporter::AssetImporter(Engine* engine) : Super()
	, engine(engine)
{

}

AssetImporter::~AssetImporter()
{

}

TRefPtr<StaticMesh> AssetImporter::ImportStaticMesh(TRefPtr<String> path)
{
	AssimpParser parser(engine);
	if (!parser.TryParse(path))
	{
		return nullptr;
	}

	return parser.GetStaticMesh();
}