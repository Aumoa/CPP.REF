// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "EngineSubsystems/GameAssetSystem.h"

SGameObject::SGameObject() : Super()
{
}

std::wstring SGameObject::ToString() const
{
	return GetName();
}

std::wstring SGameObject::GetName() const
{
	if (_name.length() == 0)
	{
		_name = StringUtils::AsUnicode(typeid(*this).name());
	}
	return _name;
}

SWorld* SGameObject::GetWorld() const
{
	if (_WorldPrivate == nullptr)
	{
		// Caching world instance with outer chain.
		if (auto* isGameObject = dynamic_cast<SGameObject*>(GetOuter()); isGameObject != nullptr)
		{
			_WorldPrivate = isGameObject->GetWorld();
		}
	}

	return _WorldPrivate;
}

void SGameObject::SetWorld(SWorld* value)
{
	_WorldPrivate = value;
}

SObject* SGameObject::LoadObject(const std::filesystem::path& assetPath)
{
	return GEngine->GetEngineSubsystem<SGameAssetSystem>()->LoadObject(assetPath);
}