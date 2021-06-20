// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"

class GameEngine;
class StaticMesh;
class RHIVertexFactory;

class AssetImporter : virtual public Object
{
public:
	using Super = Object;

private:
	GameEngine* _engine = nullptr;
	RHIVertexFactory* _factory = nullptr;

public:
	AssetImporter(GameEngine* engine, RHIVertexFactory* factory);

	StaticMesh* ImportStaticMesh(const std::filesystem::path& importPath);
};