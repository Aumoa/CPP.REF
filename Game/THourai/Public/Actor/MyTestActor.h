// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"
#include "Framework/Actor.h"

class StaticMeshComponent;

class TH_API AMyTestActor : public AActor
{
public:
	using Super = AActor;
	using This = AMyTestActor;

private:
	TRefPtr<StaticMeshComponent> staticMeshComponent;

public:
	AMyTestActor();
	~AMyTestActor();

	void Tick(std::chrono::duration<double> deltaTime) override;
};