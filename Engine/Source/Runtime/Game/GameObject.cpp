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

World* GameObject::GetWorld() const
{
	if (_WorldPrivate == nullptr)
	{
		// Caching world instance with outer chain.
		if (auto* isGameObject = dynamic_cast<GameObject*>(GetOuter()); isGameObject != nullptr)
		{
			_WorldPrivate = isGameObject->GetWorld();
		}
	}

	return _WorldPrivate;
}

void GameObject::SetWorld(World* value)
{
	_WorldPrivate = value;
}