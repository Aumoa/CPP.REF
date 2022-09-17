// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Controllers/AylaPlayerController.h"
#include "AylaPlayerController.gen.cpp"

AAylaPlayerController::AAylaPlayerController()
{
	Log::Info(LogTemp, TEXT("AylaPlayerController spawned."));
}

void AAylaPlayerController::Tick(const TimeSpan& InDeltaTime)
{
	Super::Tick(InDeltaTime);
}