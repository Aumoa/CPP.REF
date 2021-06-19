// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "StaticMesh.h"

class RHIVertexFactory;

class StaticMeshTest : public StaticMesh
{
public:
	using Super = StaticMesh;

public:
	StaticMeshTest(std::wstring_view name, RHIVertexFactory* vfactory);
};