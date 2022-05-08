// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameObject.h"
#include "GameEngine.h"
#include "Level/Level.h"

using namespace ::libty;

SGameObject::SGameObject() : Super()
{
}

String SGameObject::ToString()
{
	return Name;
}

SGameObject* SGameObject::GetOuter() const
{
	return Outer;
}

void SGameObject::SetOuter(SGameObject* Value)
{
	Outer = Value;
}

StringView SGameObject::GetName() const
{
	return Name;
}

void SGameObject::SetName(StringView Value)
{
	Name = Value;
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
	//SObject* LoadedObject = GEngine->GetEngineSubsystem<SGameAssetSystem>()->LoadObject(assetPath);
	//if (LoadedObject)
	//{
	//	return LoadedObject;
	//}
	//else
	//{
		return nullptr;
	//}
}