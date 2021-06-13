// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Game;

using namespace std;

StaticMesh::StaticMesh(wstring_view name) : Super()
	, _name(name)
{
}