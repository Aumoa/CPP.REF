// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GameObject.h"

GameObject::GameObject()
{
}

void GameObject::Rename(std::shared_ptr<GameObject> InOuter, Name InName)
{
	Outer = InOuter;
	ObjectName = InName;
}