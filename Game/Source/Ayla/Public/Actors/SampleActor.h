// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Actor.h"
#include "SampleActor.gen.h"

SCLASS()
class AYLA_API ASampleActor : public AActor
{
	GENERATED_BODY()

public:
	ASampleActor();

protected:
	virtual void Tick(const TimeSpan& InDeltaTime) override;
};