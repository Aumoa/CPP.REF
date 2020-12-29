// Copyright 2020 Aumoa.lib. All right reserved.

#include "Levels/StartupLevel.h"

#include "Framework/StaticMeshActor.h"

StartupLevel::StartupLevel()
{

}

StartupLevel::~StartupLevel()
{

}

void StartupLevel::LoadLevel()
{
	SpawnActorPersistent<AStaticMeshActor>();
}