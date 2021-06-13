// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Core;
import SC.Runtime.Game;

using namespace std;

GameObject::GameObject() : Super()
{
}

wstring GameObject::ToString() const
{
	return GetName();
}

wstring GameObject::GetName() const
{
	if (_name.length() == 0)
	{
		_name = StringUtils::AsUnicode(typeid(*this).name());
	}
	return _name;
}