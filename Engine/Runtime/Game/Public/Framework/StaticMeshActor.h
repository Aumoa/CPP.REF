// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Actor.h"

class GStaticMeshComponent;

class GAME_API AStaticMeshActor : public AActor
{
public:
	using Super = AActor;
	using This = AStaticMeshActor;

private:
	GStaticMeshComponent* staticMeshComponent;

public:
	AStaticMeshActor();
	~AStaticMeshActor();

	vs_property_get(GStaticMeshComponent*, StaticMesh);
};