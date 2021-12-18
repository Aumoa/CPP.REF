// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameObject.h"
#include "GameEngine.h"
#include "EngineSubsystems/GameAssetSystem.h"
#include "Level/Level.h"

GENERATE_BODY(SGameObject);

SGameObject::SGameObject() : Super()
{
}

std::wstring SGameObject::ToString()
{
	return GetName();
}

void SGameObject::SetOuter(SGameObject* NewOuter)
{
	Outer = NewOuter;
}

SGameObject* SGameObject::GetOuter()
{
	return Outer;
}

void SGameObject::SetName(std::wstring_view InName)
{
	Name = InName;
}

std::wstring SGameObject::GetName()
{
	return Name;
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
	if (LoadedObject)
	{
		return LoadedObject;
	}
	else
	{
		return nullptr;
	}
}