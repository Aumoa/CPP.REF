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
		{ Vector3(1, -1, -0.5f), Color::Red },
		{ Vector3(1, 1, -0.5f), Color::Green },
		{ Vector3(-1, 1, -0.5f), Color::Blue },
		{ Vector3(-1, -1, -0.5f), Color::White }
	};

	uint32 indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	myStaticMesh = StaticMesh::CreateStaticMesh(vertices, indices);
	staticMeshComponent->SetStaticMesh(myStaticMesh.Get());
}

APlane::~APlane()
{

}