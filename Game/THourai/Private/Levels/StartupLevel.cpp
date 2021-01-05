// Copyright 2020 Aumoa.lib. All right reserved.

#include "Levels/StartupLevel.h"

#include "Actor/MyTestActor.h"
#include "Pawn/MyCharacter.h"

StartupLevel::StartupLevel()
{

}

StartupLevel::~StartupLevel()
{

}

void StartupLevel::LoadLevel()
{
	SpawnActorPersistent<AMyTestActor>();
	SpawnActorPersistent<AMyCharacter>();
}