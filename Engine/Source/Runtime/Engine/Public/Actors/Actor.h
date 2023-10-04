// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "Actor.generated.h"

ACLASS()
class ENGINE_API AActor : public AObject
{
	GENERATED_BODY()

private:
	Name MyName;
	WeakPtr<AObject> Outer;

public:
	AActor();

	bool Rename(Name NewName, AObject* NewOuter);
	Name GetName() const { return MyName; }

	static void Destroy(AActor* InActor);
};