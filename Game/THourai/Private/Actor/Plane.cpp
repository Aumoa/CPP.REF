// Copyright 2020 Aumoa.lib. All right reserved.

#include "Actor/Plane.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SpringArmComponent.h"
#include "Components/CameraComponent.h"
#include "SceneRendering/StaticMesh.h"

using namespace std;
using namespace std::chrono;

APlane::APlane() : Super()
	, staticMeshComponent(nullptr)

	, springArm(nullptr)
	, camera(nullptr)
{
	staticMeshComponent = AddComponent<StaticMeshComponent>();
	RootComponent = staticMeshComponent;

	RHIVertex vertices[] =
	{
		{ Vector3(1, 0, 0), Color::Red },
		{ Vector3(0, 1, -2), Color::Green },
		{ Vector3(0, -1, -2), Color::Blue }
	};

	uint32 indices[] =
	{
		0, 1, 2
	};

	myStaticMesh = StaticMesh::CreateStaticMesh(vertices, indices);
	staticMeshComponent->SetStaticMesh(myStaticMesh.Get());
}

APlane::~APlane()
{

}