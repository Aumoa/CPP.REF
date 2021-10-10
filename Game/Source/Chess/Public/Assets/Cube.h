// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Assets/StaticMesh.h"

class SGameEngine;

class CHESS_API Cube : public SStaticMesh
{
	GENERATED_BODY(Cube)

public:
	Cube(std::wstring_view name, SGameEngine* engine);

private:
	static SStaticMeshRenderData* GetRenderData(SGameEngine* engine);
};