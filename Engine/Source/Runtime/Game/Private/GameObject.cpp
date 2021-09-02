// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "EngineSubsystems/GameAssetSystem.h"

GameObject::GameObject() : Super()
{
}

std::wstring GameObject::ToString() const
{
	return GetName();
}

std::wstring GameObject::GetName() const
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

Object* GameObject::LoadObject(const std::filesystem::path& assetPath)
{
	return GEngine->GetEngineSubsystem<GameAssetSystem>()->LoadObject(assetPath);
}