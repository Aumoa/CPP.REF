// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Level/ActorIterator.h"
#include "Level/World.h"
#include "Level/Level.h"

ActorIterator::ActorIterator(SWorld* InWorld)
	: MyWorld(InWorld)
	, CurrentLevel(InWorld->Level)
	, CurrentIdx(0)
{
}

AActor* ActorIterator::operator *()
{
	return CurrentLevel->Actors[CurrentIdx];
}

ActorIterator& ActorIterator::operator ++()
{
	++CurrentIdx;
	return *this;
}

bool ActorIterator::operator !=(const ActorIterator&) const
{
	return CurrentIdx != (int32)CurrentLevel->Actors.size();
}