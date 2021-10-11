// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameObject.h"
#include "GameEngine.h"
#include "EngineSubsystems/GameAssetSystem.h"
#include "Level/World.h"

SGameObject::SGameObject() : Super()
{
}

std::wstring SGameObject::ToString(std::wstring_view formatArgs)
{
	return GetName();
}

std::wstring SGameObject::GetName()
{
	if (_name.length() == 0)
	{
		_name = StringUtils::AsUnicode(typeid(*this).name());
	}
	return _name;
}

SWorld* SGameObject::GetWorld()
{
	if (_WorldPrivate == nullptr)
	{
		if (auto* isWorld = Cast<SWorld>(GetOuter()); isWorld != nullptr)
		{
			_WorldPrivate = isWorld;
		}

		// Caching world instance with outer chain.
		else if (auto* isGameObject = Cast<SGameObject>(GetOuter()); isGameObject != nullptr)
		{
			_WorldPrivate = isGameObject->GetWorld();
		}
	}

	return _WorldPrivate;
}

SObject* SGameObject::LoadObject(const std::filesystem::path& assetPath)
{
	return GEngine->GetEngineSubsystem<SGameAssetSystem>()->LoadObject(assetPath);
}