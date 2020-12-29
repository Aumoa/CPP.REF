// Copyright 2020 Aumoa.lib. All right reserved.

#include "Levels/StartupLevel.h"

#include "Actor/MyTestActor.h"

StartupLevel::StartupLevel()
{

}

StartupLevel::~StartupLevel()
{

}

void StartupLevel::LoadLevel()
{
	SpawnActorPersistent<AMyTestActor>();
}