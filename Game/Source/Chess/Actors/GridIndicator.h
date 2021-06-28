// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "GameFramework/Actor.h"

class StaticMesh;
class StaticMeshComponent;

class AGridIndicator : public AActor
{
public:
	using Super = AActor;

private:
	StaticMesh* _MeshObject = nullptr;
	StaticMeshComponent* _StaticMeshComponent = nullptr;

public:
	AGridIndicator();
};