// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Core;
import SC.Runtime.Game;

using namespace std;

GameObject::GameObject(wstring_view name) : Super()
	, _name(name)
{
}

wstring GameObject::ToString() const
{
	return _name;
}