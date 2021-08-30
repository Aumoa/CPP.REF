// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <filesystem>
#include "RenderMinimal.h"

class GameEngine;
class StaticMesh;
class RHIVertexFactory;
class Asset;

class GAME_API AssetImporter : virtual public Object
{
	CLASS_BODY(AssetImporter)

private:
	struct LoadedObjectData
	{
		Asset* Ptr = nullptr;
	};

private:
	GameEngine* _engine = nullptr;
	RHIVertexFactory* _factory = nullptr;
	std::map<std::filesystem::path, LoadedObjectData> _assets;

public:
	AssetImporter(GameEngine* engine, RHIVertexFactory* factory);

	void SearchContents();
	Asset* LoadObject(const std::filesystem::path& importPath);
	void UnloadObject(const std::filesystem::path& importPath);

private:
	Asset* LoadAssimpObject(const std::filesystem::path& importPath);
};