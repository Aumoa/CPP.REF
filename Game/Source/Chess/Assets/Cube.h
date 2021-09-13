// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "Assets/StaticMesh.h"

class SGameEngine;

class Cube : public SStaticMesh
{
public:
	using Super = SStaticMesh;

public:
	Cube(std::wstring_view name, SGameEngine* engine);

private:
	static SStaticMeshRenderData* GetRenderData(SGameEngine* engine);
};