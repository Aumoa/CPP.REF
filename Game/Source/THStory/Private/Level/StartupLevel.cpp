// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Level/StartupLevel.h"
#include "THGameMode.h"
#include "Actors/SampleActor.h"

SStartupLevel::SStartupLevel() : Super()
{
	GameModeClass = ATHGameMode::StaticClass();
}

void SStartupLevel::OnLoadLevel()
{
	ASampleActor* SampleActors[100] = {};

	for (size_t i = 0; i < std::size(SampleActors); ++i)
	{
		SampleActors[i] = SpawnActor<ASampleActor>();
		SampleActors[i]->SetName(std::format(L"SampleActor_{}", i));
	}

	SampleActors[44]->PrimaryActorTick.Prerequisites.emplace_back(&SampleActors[77]->PrimaryActorTick);
	SampleActors[11]->PrimaryActorTick.Prerequisites.emplace_back(&SampleActors[44]->PrimaryActorTick);
	SampleActors[2]->PrimaryActorTick.Prerequisites.emplace_back(&SampleActors[45]->PrimaryActorTick);
}