// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:StaticMesh;

import std.core;
import SC.Runtime.Core;

using namespace std;

export class StaticMesh : virtual public Object
{
public:
	using Super = Object;

public:
	StaticMesh(wstring_view name);

	wstring GetName() const;

protected:

};