// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameEngineSubsystem.h"

using namespace ::libty;

SGameEngineSubsystem::SGameEngineSubsystem() : Super()
{
}

SGameEngineSubsystem::~SGameEngineSubsystem()
{
}

Task<> SGameEngineSubsystem::StartAsync(std::stop_token CancellationToken)
{
	return Task<>::CompletedTask();
}

Task<> SGameEngineSubsystem::StopAsync(std::stop_token CancellationToken)
{
	return Task<>::CompletedTask();
}

void SGameEngineSubsystem::Tick(float InDeltaTime)
{
}