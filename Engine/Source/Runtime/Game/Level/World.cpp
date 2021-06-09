// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Core;
import SC.Runtime.Game;

using enum ELogVerbosity;

World::World()
{
}

World::~World()
{
}

bool World::InternalSpawnActor(AActor* instance)
{
	return _actors.emplace(instance).second;
}

bool World::LoadLevel(SubclassOf<Level> levelToLoad)
{
	if (!levelToLoad.IsValid())
	{
		LogSystem::Log(LogWorld, Error, L"The parameter that specified class of desired to load level is nullptr. Abort.");
		return false;
	}

	// Clear spawned actors.
	for (auto& actor : _actors)
	{
		DestroySubobject(actor);
	}
	_actors.clear();

	Level* levelInstance = levelToLoad.Instantiate(this);
	if (!levelInstance->LoadLevel(this))
	{
		LogSystem::Log(LogWorld, Fatal, L"Could not load level.");
		return false;
	}

	return true;
}