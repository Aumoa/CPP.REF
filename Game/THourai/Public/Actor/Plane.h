// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"
#include "Framework/Actor.h"

class StaticMesh;
class StaticMeshComponent;
class SpringArmComponent;
class CameraComponent;

class TH_API APlane : public AActor
{
public:
	using Super = AActor;
	using This = APlane;

private:
	TRefPtr<StaticMesh> myStaticMesh;
	StaticMeshComponent* staticMeshComponent;

	SpringArmComponent* springArm;
	CameraComponent* camera;

public:
	APlane();
	~APlane();
};