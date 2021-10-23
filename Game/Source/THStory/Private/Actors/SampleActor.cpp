// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Actors/SampleActor.h"
#include "LogTHStory.h"

ASampleActor::ASampleActor() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASampleActor::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
	AccumulateTime += InDeltaTime;

	if (AccumulateTime > 5.0f)
	{
		SE_LOG(LogTH, Verbose, L"{}: ASampleActor::Tick() called.", GetName());
		AccumulateTime -= 5.0f;
	}
}