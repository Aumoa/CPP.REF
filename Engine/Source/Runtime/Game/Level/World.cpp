// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Core;
import SC.Runtime.Game;

World::World()
{
}

World::~World()
{
}

bool World::InternalSpawnActor(AActor* instance)
{
	return actors.emplace(instance).second;
}