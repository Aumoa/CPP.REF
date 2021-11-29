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

void SGameObject::SetName(std::wstring_view InName)
{
	_Name = InName;
}

std::wstring SGameObject::GetName()
{
	return _Name;
}

SLevel* SGameObject::GetLevel()
{
	return Cast<SLevel>(GetOuter());
}

SWorld* SGameObject::GetWorld()
{
	if (SLevel* Level = GetLevel())
	{
		return Level->GetWorld();
	}
	return nullptr;
}

SObject* SGameObject::LoadObject(const std::filesystem::path& assetPath)
{
	SObject* LoadedObject = GEngine->GetEngineSubsystem<SGameAssetSystem>()->LoadObject(assetPath);
	LoadedObject->SetOuter(this);
	return LoadedObject;
}