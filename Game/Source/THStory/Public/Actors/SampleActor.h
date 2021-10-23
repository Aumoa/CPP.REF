// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class THSTORY_API ASampleActor : public AActor
{
	GENERATED_BODY(ASampleActor)

private:
	float AccumulateTime = 0;

public:
	ASampleActor();

	virtual void Tick(float InDeltaTime) override;
};