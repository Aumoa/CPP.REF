// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameEngineSubsystem.h"

GENERATE_BODY(SGameEngineSubsystem);

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