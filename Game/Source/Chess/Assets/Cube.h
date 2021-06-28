// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "Assets/StaticMesh.h"

class GameEngine;

class Cube : public StaticMesh
{
public:
	using Super = StaticMesh;

public:
	Cube(std::wstring_view name, GameEngine* engine);

private:
	static StaticMeshRenderData* GetRenderData(GameEngine* engine);
};