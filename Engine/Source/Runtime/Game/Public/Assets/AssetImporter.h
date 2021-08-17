// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <filesystem>
#include "RenderMinimal.h"

class GameEngine;
class StaticMesh;
class RHIVertexFactory;

class GAME_API AssetImporter : virtual public Object
{
public:
	using Super = Object;

private:
	GameEngine* _engine = nullptr;
	RHIVertexFactory* _factory = nullptr;
	std::map<std::filesystem::path, Object*> _assets;

public:
	AssetImporter(GameEngine* engine, RHIVertexFactory* factory);

	void SearchContents();

	StaticMesh* ImportStaticMesh(const std::filesystem::path& importPath);
};