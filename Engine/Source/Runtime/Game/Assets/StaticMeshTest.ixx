// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:StaticMeshTest;

import std.core;
import SC.Runtime.RenderCore;
import :StaticMesh;

using namespace std;

export class StaticMeshTest : public StaticMesh
{
public:
	using Super = StaticMesh;

public:
	StaticMeshTest(wstring_view name, RHIVertexFactory* vfactory);
};