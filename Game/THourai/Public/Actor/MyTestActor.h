// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"
#include "Framework/Actor.h"

class StaticMesh;
class StaticMeshComponent;
class SpringArmComponent;
class CameraComponent;

class TH_API AMyTestActor : public AActor
{
public:
	using Super = AActor;
	using This = AMyTestActor;

private:
	TRefPtr<StaticMesh> myStaticMesh;
	StaticMeshComponent* staticMeshComponent;

	SpringArmComponent* springArm;
	CameraComponent* camera;

public:
	AMyTestActor();
	~AMyTestActor();

	void Tick(Seconds deltaTime) override;
};