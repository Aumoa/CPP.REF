// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameEngineSubsystem.h"

using namespace ::libty;

SGameEngineSubsystem::SGameEngineSubsystem() : Super()
{
}

SGameEngineSubsystem::~SGameEngineSubsystem()
{
}

void SGameEngineSubsystem::Deinit()
{
	GC.SuppressFinalize(this);
}