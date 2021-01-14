// Copyright 2020 Aumoa.lib. All right reserved.

#include "Actor/MyTestActor.h"

#include "Components/StaticMeshComponent.h"
#include "SceneRendering/StaticMesh.h"

using namespace std;
using namespace std::chrono;

AMyTestActor::AMyTestActor() : Super()
	, staticMeshComponent(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	staticMeshComponent = AddComponent<StaticMeshComponent>();
	RootComponent = staticMeshComponent;

	RHIVertex vertices[3] =
	{
		{ Vector3(1, 1, 0), Color::Red },
		{ Vector3(1, -1, 0), Color::Green },
		{ Vector3(-1, -1, 0), Color::Blue }
	};

	uint32 indices[3] =
	{
		0, 1, 2
	};

	myStaticMesh = StaticMesh::CreateStaticMesh(vertices, indices);
	staticMeshComponent->SetStaticMesh(myStaticMesh.Get());
}

AMyTestActor::~AMyTestActor()
{

}

void AMyTestActor::Tick(Seconds deltaTime)
{
	Super::Tick(deltaTime);
}