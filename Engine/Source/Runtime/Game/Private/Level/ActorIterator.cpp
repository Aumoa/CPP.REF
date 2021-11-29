// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Level/ActorIterator.h"
#include "Level/World.h"
#include "Level/Level.h"

ActorIterator::ActorIterator(SWorld* InWorld)
	: _MyWorld(InWorld)
	, _CurrentLevel(InWorld->_Level)
	, _CurrentIdx(0)
{
}

AActor* ActorIterator::operator *()
{
	return _CurrentLevel->Actors[_CurrentIdx];
}

ActorIterator& ActorIterator::operator ++()
{
	++_CurrentIdx;
	return *this;
}

bool ActorIterator::operator !=(const ActorIterator&) const
{
	return _CurrentIdx != (int32)_CurrentLevel->Actors.size();
}