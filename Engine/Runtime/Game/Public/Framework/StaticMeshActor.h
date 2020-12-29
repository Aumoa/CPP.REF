// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Actor.h"

class StaticMeshComponent;

class GAME_API AStaticMeshActor : public AActor
{
public:
	using Super = AActor;
	using This = AStaticMeshActor;

private:
	StaticMeshComponent* staticMeshComponent;

public:
	AStaticMeshActor();
	~AStaticMeshActor();
};