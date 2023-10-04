// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Actors/Actor.h"

AActor::AActor()
{
}

bool AActor::Rename(Name NewName, AObject* NewOuter)
{
	RefPtr<AObject> OuterLock = Outer.Lock();
	if (OuterLock == NewOuter && MyName == NewName)
	{
		return false;
	}

	MyName = NewName;
	Outer = NewOuter;
	return true;
}