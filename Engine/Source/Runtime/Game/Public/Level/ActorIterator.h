// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SWorld;
class SLevel;
class AActor;

class ActorIterator
{
	SWorld* MyWorld;
	SLevel* CurrentLevel;
	int32 CurrentIdx;

public:
	ActorIterator(SWorld* InWorld);

	AActor* operator *();
	ActorIterator& operator ++();
	bool operator !=(const ActorIterator&) const;

	ActorIterator& begin() { return *this; }
	ActorIterator& end() { return *this; }
};