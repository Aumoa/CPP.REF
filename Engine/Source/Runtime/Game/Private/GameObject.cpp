// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameObject.h"
#include "GameEngine.h"
#include "EngineSubsystems/GameAssetSystem.h"
#include "Level/Level.h"

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
		if (auto* IsLevel = Cast<SLevel>(GetOuter()); IsLevel != nullptr)
		{
			_WorldPrivate = IsLevel->GetWorld();
		}

		else if (auto* IsGameObject = Cast<SGameObject>(GetOuter()); IsGameObject != nullptr)
		{
			// Do NOT cache world when world is derived from outer chain that is game object.
			return IsGameObject->GetWorld();
		}
	}

	return _WorldPrivate;
}

SObject* SGameObject::LoadObject(const std::filesystem::path& assetPath)
{
	return GEngine->GetEngineSubsystem<SGameAssetSystem>()->LoadObject(assetPath);
}