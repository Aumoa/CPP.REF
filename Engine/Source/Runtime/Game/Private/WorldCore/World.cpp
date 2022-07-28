// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "WorldCore/World.h"
#include "EngineCore/Engine.h"
#include "Ticking/LevelTick.h"
#include "World.gen.cpp"

World::World(Engine* engine) : Super()
	, _engine(engine)
{
	_levelTick = gcnew LevelTick(this);
}

Engine* World::GetOuter()
{
	return _engine;
}

void World::DispatchWorldTick(const TimeSpan& deltaTime)
{
	_levelTick->BeginFrame();
	_levelTick->IncrementalDispatchTick(ETickingGroup::PrePhysics, deltaTime);
	_levelTick->IncrementalDispatchTick(ETickingGroup::DuringPhysics, deltaTime);
	_levelTick->IncrementalDispatchTick(ETickingGroup::PostPhysics, deltaTime);
	_levelTick->IncrementalDispatchTick(ETickingGroup::PostUpdateWork, deltaTime);
	_levelTick->EndFrame();
}